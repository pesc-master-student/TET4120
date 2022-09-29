//--------------------------------------------------
// This file contains board config data, and some IO definition for a generic template hardware project.
// GPIO, LEDS, relay drivers etc:
//
// 	PicoZed 7030 based processor board.  Sintef Energy.
//
//-----------------------------------------------------------
#include "xparameters.h"


//-------------------------------------------------------------

#define PICOZED_7030_BASED_PROCESSOR_BOARD


//-------------------------------------------------------------

#define  SYNCH_IO  //  isync; __sync; // Synkroniserer data.


#define Xil_EnableNestInterrupts(void)			// dummy
#define Xil_DisableNestInterrupts(void)			// dummy

//-------------------------------------------------------------
// Clocks timer

#define FPGA_SYSTEM_CLOCK_FREQUENCY 			100000000U
#define CPU_CLOCK_FREQUENCY 					XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ

#define SYSTEM_CLOCK_FREQUENCY					(CPU_CLOCK_FREQUENCY/2)


#define System_clock_startup()  					// Starts automatically.

#define SYSTEM_CLOCK 							*(volatile unsigned int*)(XPAR_GLOBAL_TMR_BASEADDR + 0*1) // Lower 32 bits.

#define AXI_TIMER_BASEADRESSE					XPAR_AXI_TIMER_0_BASEADDR	// Axi timer


//-------------------------------------------------
// FPGA interrupts

#define ZYNQ_INT_HANDLER  	//	type.

#define AXI_TIMER_INT_FPGA_VEC_BITNR			0


//-------------------------------
// UART, Stdio. Assumes STDIN uses   XPAR_PS7_UART_0_BASEADDR


#define UART_BASE_ADR							XPAR_PS7_UART_0_BASEADDR

#define STDIO_UART_TYPE_ZYNQ_UARTPS				// Determines which stdin_is_empty_low level function to use.


//----------------------
// Parameter Eeprom

#define EEPROM_ZYNQ_SPI  //spi port type

#define EEPROM_SPI_DEVICE_ID					XPAR_XSPIPS_0_DEVICE_ID
#define EEPROM_SPI_SELECT						00 // CS velger.
#define EEPROM_SIZE								128000	// 64 kbyte.  minst.
#define EEPROM_STARTADR							0	//Indirekte, via SPI port.

//-------------------------------------------
// System monitor

#define  SYSMON_BASEADR    						XPAR_SYSMON_0_BASEADDR


#define SYSMON_SIGNAL_A 						SYSMON_AD_IN(0)
#define SYSMON_SIGNAL_B 						SYSMON_AD_IN(8)
#define SYSMON_SIGNAL_C 						SYSMON_AD_IN(1)
#define SYSMON_SIGNAL_D							SYSMON_AD_IN(9)
#define SYSMON_SIGNAL_E							SYSMON_AD_IN(2)
#define SYSMON_SIGNAL_F							SYSMON_AD_IN(10)

#define SYSMON_P15V								SYSMON_AD_IN(3)
#define SYSMON_N15V								SYSMON_AD_IN(11)

#define SYSMON_N3V								SYSMON_VP_VN


#define SYSMON_SIGNAL_TOP						5000 // [mV]

#define SYSMON_VCCINT_TOP						3000 // [mV]
#define SYSMON_VCCAUX_TOP						3000 // [mV]

#define SYSMON_N3V_TOP							(-5000) // [mV]

#define SYSMON_P15V_TOP							20000 // [mV]
#define SYSMON_N15V_TOP							(-20000) // [mV]


#define SYSMON_TEMP_TOP			  				230 // [K]
#define SYSMON_TEMP_BOTTOM					 	(-273) // [C]

//---
// sysmon configuration settings.

#define SYSMON_AD_CLOCK_FREQUENCY_MHZ 5000000  // 25 MHz for Artix7, 5 MHZ for Virtex5.  5 MHZ max for PicoZed using external multiplexers, due to crosstalk.

#define SYSMON_CONFIG0_DATA	0x3910  //averaging on.  external multiplexer.

#define SYSMON_CONFIG1_DATA	0x4100	// Simult. sampling,


//-------------------------------------------------------------------------------------
// Serial AD converter.

#define AD_RAW_SAMPLE_RATE		40000000

#define AD_S_IO_DELAY_STEPS	32	//  Virtex5: 64 steps,  7-series: 32 steps.

#define AD_S_BASEADDR 							XPAR_AD_CONVERTER_BASEADDR


#define AD_S_NO_OF_CHANNELS						XPAR_AD_CONVERTER_NUMBER_OF_CHANNELS

#define AD_S_SAMPLING_RATE						(AD_RAW_SAMPLE_RATE  / (1<< ( XPAR_AD_CONVERTER_DOWNSAMPLING_FACTOR_WIDTH))) // Hz

#define AD_S_SIGNAL_WIDTH 						XPAR_AD_BLOCK_AD_CONVERTER_SERIAL_RECEIVER_0_WIDTH_OUT

#define AD_S_FULL_SCALE 						((1 << (AD_S_SIGNAL_WIDTH-1-1))-1)// Fortegnstall. Trekker fra et bit ekstra når det er offsetkompensering.

#define AD_S_CLOCK_IO_DELAY_VALUE		       XPAR_AD_CONVERTER_AD_CLOCK_IOBDELAY_VALUE
#define AD_S_IO_DELAY_INITVALUE				  (XPAR_AD_CONVERTER_AD_SIGNAL_IOBDELAY_VALUE - XPAR_AD_CONVERTER_AD_CLOCK_IOBDELAY_VALUE)

/*

#define AD_S_BASEADDR 							XPAR_AD_OMFORMER_SERIEMOTTAKER_0_BASEADDR


#define AD_S_NO_OF_CHANNELS						XPAR_AD_OMFORMER_SERIEMOTTAKER_0_ANTALL_KANALER

#define AD_S_SAMPLING_RATE						(AD_RAW_SAMPLE_RATE  / (1<< ( XPAR_AD_OMFORMER_SERIEMOTTAKER_0_NEDSAMPLINGSFAKTOR_BREDDE))) // Hz

#define AD_S_SIGNAL_BREDDE 						XPAR_AD_OMFORMER_SERIEMOTTAKER_0_BREDDE_UT

#define AD_S_FULL_SCALE 						((1 << (AD_S_SIGNAL_BREDDE-1-1))-1)// Fortegnstall. Trekker fra et bit ekstra når det er offsetkompensering.

#define AD_S_CLOCK_IO_DELAY_VALUE		XPAR_AD_OMFORMER_SERIEMOTTAKER_0_AD_CLOCK_IOBDELAY_VALUE
#define AD_S_IO_DELAY_INITVALUE				(XPAR_AD_OMFORMER_SERIEMOTTAKER_0_AD_SIGNAL_IOBDELAY_VALUE - XPAR_AD_OMFORMER_SERIEMOTTAKER_0_AD_CLOCK_IOBDELAY_VALUE)

*/



//--------------------------------------------------------------------
// DA converters.

#define DA_BASEADR								XPAR_DA_CONVERTER_BASEADDR

#define DA_CONVERTER_WIDTH_IN					XPAR_DA_CONVERTER_WIDTH_IN
#define DA_CONVERTER_FULL_SCALE_RAW				((1<<(XPAR_DA_CONVERTER_WIDTH_IN-1))-1) // Fortegnstall.
#define DA_CONVERTER_NO_OF_HW_SIGNALS_IN  		XPAR_DA_CONVERTER_NUMBER_OF_SIGNAL_SOURCES
#define DA_CONVERTER_SCALING_SHIFTS				XPAR_DA_CONVERTER_SCALE_SHIFTS
/*

#define DA_BASEADR								XPAR_DA_OMFORMER_UTGANG_0_BASEADDR

#define DA_CONVERTER_WIDTH_IN					XPAR_DA_OMFORMER_UTGANG_0_BREDDE_INN
#define DA_CONVERTER_FULL_SCALE_RAW				((1<<(DA_CONVERTER_WIDTH_IN-1))-1) // Fortegnstall.
#define DA_CONVERTER_NO_OF_HW_SIGNALS_IN  		XPAR_DA_OMFORMER_UTGANG_0_ANTALL_SIGNALKILDER
#define DA_CONVERTER_SCALING_SHIFTS				XPAR_DA_OMFORMER_UTGANG_0_SKALERING_SKYV
*/

//-------------------------------
// Relays

#define NUMBER_OF_RELAYS						4

#define RELAY_DRIVER_REG 						*(volatile int*)(XPAR_RELAY_BASEADDR + 0)

#define RELAY_A_ON  							RELAY_DRIVER_REG = RELAY_DRIVER_REG | 0x01
#define RELAY_A_OFF   							RELAY_DRIVER_REG = RELAY_DRIVER_REG & (~0x01)

#define RELAY_B_ON  							RELAY_DRIVER_REG = RELAY_DRIVER_REG | 0x02
#define RELAY_B_OFF   							RELAY_DRIVER_REG = RELAY_DRIVER_REG & (~0x02)


#define RELAY_C_ON  							RELAY_DRIVER_REG = RELAY_DRIVER_REG | 0x04
#define RELAY_C_OFF   							RELAY_DRIVER_REG = RELAY_DRIVER_REG & (~0x04)


#define RELAY_D_ON  							RELAY_DRIVER_REG = RELAY_DRIVER_REG | 0x08
#define RELAY_D_OFF   							RELAY_DRIVER_REG = RELAY_DRIVER_REG & (~0x08)


//-----------------------------------------------------
// Testsignals, leds

#define NUMBER_OF_TESTLEDS						5

#define TESTLED_REG								*(volatile int*)(XPAR_TESTLED_BASEADDR + 0) 	//-- bit 0: A-rød,	1: B-orange, 2: C-gul,	3: D-gronn, 5: E-blågrønn, 6: F-blå

#define TESTLED_RED_ON  						TESTLED_REG = TESTLED_REG | 0x01
#define TESTLED_RED_OFF   						TESTLED_REG = TESTLED_REG & (~0x01)

#define TESTLED_ORANGE_ON  						TESTLED_REG = TESTLED_REG | 0x02
#define TESTLED_ORANGE_OFF   					TESTLED_REG = TESTLED_REG & (~0x02)

#define TESTLED_YELLOW_ON 						TESTLED_REG = TESTLED_REG | 0x04
#define TESTLED_YELLOW_OFF  					TESTLED_REG = TESTLED_REG & (~0x04)

#define TESTLED_GREEN_ON  						TESTLED_REG = TESTLED_REG | 0x08
#define TESTLED_GREEN_OFF   					TESTLED_REG = TESTLED_REG & (~0x08)


#define TESTLED_BLUE_ON  						TESTLED_REG = TESTLED_REG | 0x10
#define TESTLED_BLUE_OFF   						TESTLED_REG = TESTLED_REG & (~0x10)


//--------------------
// Signal inputs.

#define NUMBER_OF_SIGNALS_IN					6

#define SIGNAL_IN_D								*(volatile int*)(XPAR_SIGNAL_IN_BASEADDR + 0)   //
#define SIGNAL_IN_R_PU							*(volatile int*)(XPAR_SIGNAL_IN_BASEADDR + 8)	// 0: Opptrekksmotstand av 1: Opptrekksmotstand 1 kOhm på.

//------------------------------------
// GPIO ports


#define GPIO1_D  								*(volatile int*)(XPAR_GPIO1_BASEADDR+0)
#define GPIO1_DIR  								*(volatile int*)(XPAR_GPIO1_BASEADDR+4)

//#define GPIO2_D  								*(volatile int*)(XPAR_GPIO2_BASEADDR+0)
//#define GPIO2_DIR  							*(volatile int*)(XPAR_GPIO2_BASEADDR+4)


//----------------------------------------------
// Bus interface for LCD display, user interface.

#define GPIO2_EPC_BASEADDR						XPAR_AXI_EPC_0_PRH0_BASEADDR
#define DISPLAY_OG_TASTATUR_ADR_SHIFT		2  // LSB bit nr i adresse.    Må skyves to hakk opp, AXI-Lite bruker kun int-aksess.



//--------------------
// Additional signal inputs

#define NUMBER_OF_DIPSWITCHES					2

#define CAN_DEVICE_ID							XPAR_XCANPS_0_DEVICE_ID

#define ENCODER_BASEADR 						XPAR_ENCODER_INTERFACE_BASEADDR

//----------------------------------
// Gigabit link


#define GTX   //Type of transceiver				ARTIX, Zync7015?: GTP, Kintex, Xynq7030:  GTX,  Virtex:   GTX, GTH, GTZ

#define NUMBER_OF_GBIT_CHANNELS   				4

#define DRP_BRIDGE_BASEADR 						XPAR_GIGABIT_BLOCK_DRP_BRIDGE_0_BASEADDR
#define DRP_ADDR_WIDTH							9


//----------------------
// Gigabit link registers.

#define GIGABIT_RESET_REG						*(volatile unsigned int*)  (XPAR_GIGABIT_BLOCK_GIGABIT_REGARRAY_BASEADDR + 0)
	#define GIGABIT_RESET_PIN_BITNR 0
	#define GIGABIT_GT_RESET_PIN_BITNR 1
	//#define GIGABIT_LOOPBACK_PIN_BIT_STARTNR 4 // 3 bit

#define GIGABIT_STATUS_REG	*(volatile unsigned int*)  (XPAR_GIGABIT_BLOCK_GIGABIT_REGARRAY_BASEADDR + 4)
	#define GIGABIT_CHANNEL_UP_BITNR_START 0 // bit 0-3 Gigabit channel up signals
	#define GIGABIT_SYS_RESET_BITNR_START  8 // bit 8-11 Gigabit sys_reset signals

//--------------------
// Gigabit data fifos.

#define FIBER_FIFO_0_BASEADR				XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_0_BASEADDR
#define FIBER_FIFO_0_AXI_BASEADR			XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_0_AXI4_BASEADDR

#define FIBER_FIFO_1_BASEADR				XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_1_BASEADDR
#define FIBER_FIFO_1_AXI_BASEADR			XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_1_AXI4_BASEADDR

#define FIBER_FIFO_2_BASEADR				XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_2_BASEADDR
#define FIBER_FIFO_2_AXI_BASEADR			XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_2_AXI4_BASEADDR


#define FIBER_FIFO_3_BASEADR				XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_3_BASEADDR
#define FIBER_FIFO_3_AXI_BASEADR			XPAR_GIGABIT_BLOCK_AXI_FIFO_MM_S_3_AXI4_BASEADDR

#define FIBER_1_2_CHAIN_LINK_SWITCH_BASEADDR  XPAR_GIGABIT_BLOCK_AXI_S_CHAIN_LINK_SWITCH_0_BASEADDR


//---------------------------------------------------------------------------------------------


