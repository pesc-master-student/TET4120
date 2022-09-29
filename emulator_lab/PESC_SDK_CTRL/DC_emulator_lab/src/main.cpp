
#include <types.hpp>
#include <Datalogger.hpp>
#include "../include/DriveRoutineFast.hpp"
#include <wtcp.hpp>
#include <xil_mmu.h>
#include <xil_cache.h>
#include <xil_cache_l.h>
#include <xpseudo_asm.h>
#include <xparameters.h>
#include "../include/CommInterface.hpp"
#include "../include/StopWatch.hpp"
#include "../include/basis_io_picozed7030_based_processor_board.h"
#include "../include/Exception.hpp"
#include "../include/Initialize_SW.hpp"
#include "../include/HMI.hpp"
#include "../include/Multiplexer.hpp"


const enum Multiplexer::e_modus PhysicalMode = Multiplexer::EMULATOR; // PHYSICAL/EMULATOR
SW SW1;
DAC m_DAC;

#define USE_CPU_TO_CPU_IRQ 1
extern u32 MMUTable;
const uint16_t MAX_TX_PACKET_LENGTH = 0x2000;
const uint16_t MAX_RX_PACKET_LENGTH = 0x2000;


Wtcp g_wtcp;
StopWatch sw;
static uint8_t wtcp_tx_buffer[MAX_TX_PACKET_LENGTH];
fp32_t Time_used_PWMIsr_us;
fp32_t m_curTime = 0.0;
fp32_t Tsamp = 1.0e-3; // Time for interrupt
fp32_t angle_sin_a = 0.0;
int pwm_frequency = 1000;


void ReleaseWtcpPacket(uint32_t identifier);
void TransmitWtcpPacket(uint8_t* ppPacket, uint16_t size, uint32_t identifier);
uint16_t ReceiveWtcpPacket(uint8_t** ppPacket, uint32_t* identifier);

void SetTlbAttributes(uint32_t addr, uint32_t attrib){
	uint32_t *ptr;
	uint32_t section;

	mtcp(XREG_CP15_INVAL_UTLB_UNLOCKED, 0);
	dsb();

	mtcp(XREG_CP15_INVAL_BRANCH_ARRAY, 0);
	dsb();
	Xil_L1DCacheFlush();

	section = addr / 0x100000;
	ptr = &MMUTable + section;
	*ptr = (addr & 0xFFF00000) | attrib;
	dsb();
}

int cntr_background;

HMI hmi;

static constexpr uint32_t DATALOGGER_BUFFER_SIZE = 0x1000000; //16 MB
static uint8_t g_bufDatalogger[DATALOGGER_BUFFER_SIZE];
constexpr uint32_t DATALOGGER_PROBE_BACKGROUND = 0;
static constexpr uint32_t NUMBER_OF_STANDARD_PROBES = 1;
static constexpr int NUM_DATALOGGERS = 1;
std::array<Datalogger,NUM_DATALOGGERS> m_dataloggers;
DataloggerProbe m_dataloggerStandardProbes[NUMBER_OF_STANDARD_PROBES];


int pwm_int_cntr = 0;
void PWMIsr(void){

	sw.Start();
	++pwm_int_cntr;
	m_curTime = m_curTime+Tsamp;

	DriveRoutineFast();

	m_dataloggers[0].Run(DATALOGGER_PROBE_BACKGROUND);
	sw.Stop();
	auto dt=sw.Elapsed();
	Time_used_PWMIsr_us = 3e-3*dt;  // in us
}


int main(void){
	// Disable L1 cache for OCM ( Communication tunnel )
	SetTlbAttributes(0xFFFF0000,0x04de2);           // S=b0 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0

	//Disable access to 0x0000 0000 - 0x37FF FFFF (Linux and Golden )
	for (uint32_t i = 0; i < 0x380; ++i)
		SetTlbAttributes( 0x100000UL * i, 0x00);

	//for (uint32_t i = 0x380; i < 0x400; ++i)
	//	SetTlbAttributes(0x100000UL * i, 0x4de6);

	//Set FPGA register space to non-cached strongly ordered
	//SetTlbAttributes( 0x40000000, 0x0C02 );


	//Set FPGA register space to non-cached strongly ordered
	for (uint32_t i = 0x400; i < 0x500; ++i)
		SetTlbAttributes( 0x100000UL * i, 0x0C02);

	g_wtcp.Init();
	g_wtcp.SetBuffers(wtcp_tx_buffer,MAX_TX_PACKET_LENGTH,MAX_RX_PACKET_LENGTH);
	g_wtcp.SetApplicationIdentifier(WTCP_APPLICATION_TYPE_RAM);
	g_wtcp.SetFirmwareIdentifierStr("UNKNOWN");
	g_wtcp.SetReceiveCallback(&ReceiveWtcpPacket);
	g_wtcp.SetTransmitCallback(&TransmitWtcpPacket);
	g_wtcp.SetReleasePacketCallback(&ReleaseWtcpPacket);

	m_dataloggerStandardProbes[DATALOGGER_PROBE_BACKGROUND].SetName("PwmInt");
	m_dataloggerStandardProbes[DATALOGGER_PROBE_BACKGROUND].SetProbeType(PROBE_ASYNCHRONOUS);
	m_dataloggerStandardProbes[DATALOGGER_PROBE_BACKGROUND].SetAsynchronousVariableAddress( &m_curTime );
	m_dataloggerStandardProbes[DATALOGGER_PROBE_BACKGROUND].SetAsynchronousVariableSize(sizeof(m_curTime));
	m_dataloggerStandardProbes[DATALOGGER_PROBE_BACKGROUND].SetAsynchronousVariableType(DATALOGGER_VARIABLE_ENCODING_TYPE_FLOATING_POINT);
	m_dataloggers[0].ResetAll();
	m_dataloggers[0].SetBuffer(g_bufDatalogger,DATALOGGER_BUFFER_SIZE);
	m_dataloggers[0].SetProbes(m_dataloggerStandardProbes,NUMBER_OF_STANDARD_PROBES);
	g_wtcp.AddDataloggers(m_dataloggers.data(),NUM_DATALOGGERS);
	

	// Initialize SW and parameters from data base:
	SW1.initialize();

	// Initialize Interrupts
	// XPAR_FABRIC_PWM_1_INTR_INTR
	Exception::Instance().ScuGicPriority( XPAR_FABRIC_CONVERTER_1_PWM_INTR_INTR, 0xA0);
	Exception::Instance().ScuGicTargetCpu( XPAR_FABRIC_CONVERTER_1_PWM_INTR_INTR, 1);
	Exception::Instance().ScuGicTriggerType( XPAR_FABRIC_CONVERTER_1_PWM_INTR_INTR, true);
	Exception::Instance().RegisterHandler( XPAR_FABRIC_CONVERTER_1_PWM_INTR_INTR, PWMIsr);
	Exception::Instance().ScuGicEnableIntr( XPAR_FABRIC_CONVERTER_1_PWM_INTR_INTR);

	Exception::EnableExceptions();  // Global Enabling
	
	while(true){
		++cntr_background;
		g_wtcp.Service();
	}
	return 0;
}

[[gnu::section(".comm_interface")]] static CommunicationHandler g_commHandler;// __attribute__((section(".comm_interface")));

struct DestinationStruct {
	uint32_t address;
	uint16_t port;
};

static std::array<DestinationStruct, NUM_RECEIVE_BUFFERS> g_dest;

void ReleaseWtcpPacket(uint32_t)
{
	g_commHandler.GetControl().IncrementInputReadIndex();
}

#if USE_CPU_TO_CPU_IRQ
constexpr uint32_t GENERATE_SW_IRQ_0 = 0x01000000;
#define ICDSGIR *(volatile uint32_t*)0xF8F01F00
#endif
void TransmitWtcpPacket(uint8_t* ppPacket,uint16_t size, uint32_t identifier){
	const DestinationStruct& dst = g_dest[identifier];
	g_commHandler.AddPacket( dst.address, dst.port, size, ppPacket );
#if USE_CPU_TO_CPU_IRQ
	ICDSGIR = GENERATE_SW_IRQ_0;
#endif
}

uint16_t ReceiveWtcpPacket(uint8_t** ppPacket, uint32_t* identifier){
	ControlHeader& control = g_commHandler.GetControl();
	if( control.IsEmpty() )
		return 0;
	Packet& packet = g_commHandler.Receive();
	*ppPacket = (uint8_t*)(packet.GetBuffer().data());
	*identifier = control.GetReadIndex();
	g_dest[control.GetReadIndex() ].address = packet.GetAddress();
	g_dest[control.GetReadIndex() ].port = packet.GetPort();
	return packet.GetSize();
}

