
/************************************************************************************
::File Name: EnDat22.hpp
::Description: Driver for Heidenhain EnDat v2 compliant sensors


                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
            Department of Electric Power Engineering, NTNU 2022



::SIGNAL FLOW/PIPELINE::
§ -------- HARDWARE --------- § - FPGA/FIRMWARE - § ------ CPU/SOFTWARE ------ §


::HARDWARE DEPENDENCIES::
- Description missing

::FIRMWARE DEPENDENCIES::
- Description missing

::SOFTWARE DEPENDENCIES::
- Description missing

::MINIMUM SOFTWARE SETUP::
- Description missing

::MINIMUM FIRMWARE SETUP::
- Description missing

::NORMAL SOFTWARE USAGE::
- Description missing

::DESCRIPTION::


CONVERSION BETWEEN TWOS COMPLEMENT AND DEGREES POSTION FORMAT
  - Position value is maximum 48 bit depending on type of encoder. Thus must
    use 64-bit fixed and floating point data types to capture full resolution.
  - STEPS_PER_REV [steps/rev] :: (number of measuring steps per revolution::datasheet or EEPROM)
  - C_360   [deg per revolution]      :: constant 360 degrees
  - E22_POS [steps]      ::  2's complement position value from encoder
  - POS_DEG [deg]      ::  encoder position converted to degrees

  POS_DEG = ( E22_POS / STEPS_PER_REV ) * C_360
  E22_POS = ( POS_DEG / C_360) * STEPS_PER_REV



::RECOMMENDED LITERATURE::
These documents provide a programmers model:
- Chap09_01-H_EnDat Application Notes 722024_06_A_02_1.pdf
- db10297e_EnDat_Master_Basic_V39.pdf
- Chap04_01-H_EnDat specification 297403_05_a_02_1.pdf
- EnDat interface PCB (SINTEF)

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 thomas.haugan@ntnu.no       01.01.2018 Original code release.
1.00 thomas.haugan@ntnu.no       01.01.2018 Original code release.

*************************************************************************************
IP-CORE MEMORY MAP:

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation depedent while register offsets are static.

Offset	Register Name							  Access Register Content (32-bit)
------	---------------------------------------   ------ ----------------------
OFFSET
REGISTER		OFFSET-DEC		OFFSET-HEX		DEFAULT VALUE
Send			0		0x0		0x07000000
Receive 1
Receive 2		12		0xC		0x00000000
Receive 3
Configuration 1	5		0x5		0x00000DF0
Configuration 2	6		0x6		0x00040000
Configuration 3	7		0x7		0x0000006C
Status			8		0x8		0x40000400
Interrupt		9		0x9		0x00000000
Test 1			10		0xA		0x00000040
Test 2			11		0xB		0x00000000
Receive 4
SW Strobe
Identification	15		0xF		0xE226051B

*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
- The E22 code should be autonomous and start and run by itself
 	 	 - Draw diagram for STROBE
 	 	 - Explain Error/Fault Types and WHY/Resolution
 	 	 - Inspired by CAN-OPEN, make a struct to fill up all three E22 encoder memory data
 	 	 - Have a wait-loop inside E22 read-functions. Not good (can give infinite loop) should rather be
 	 	   handled by software, for instance if EnDat connection is lost during run-time.
 	 	 - Check if e22CASE can go straight to 6 (read position)
 	 	 - Update revision log according to GitHub
 	 	 - Init part must require the user to provide som basic knowledge of encoder,
 	 	   such as clock speed and word length.
 	 	 - HW vs SW strobing depends on wanted timing in sampling process
 	 	 - Describe how the E22 IP-CORE was configured and integrated into FPGA design
 	 	 - How to detect that nothing is connected to endat interface board
 	 	 - Implement all functions similar as from SINTEF-Encoder
 	 	 - Make subfolder "Encoder", add ROD420 and E22 Classes
 	 	 - Implement configurable DATUM-Shift
 	 	 - DEriving speed (derivative of position) contains notable ripple which
 	 	   must be low-pass filtered.
 	 	 - Remove LPFilter1
 	 	 - Add enum nd log for STILL / CW / CCW status
 	 	 - Maybe mask off LSBs => loose some resolution but theta becomes more stable (selectable)
 	 	 - Make function computing position and angular speed
 	 	 - Periodically read from encoder
 	 	 	 - S/N: detects changed encoder during runtime
 	 	 	 - Temperature inside encoder
 	 	 	 - Encoder Warning / Error bits
 	 	 	 -

************************************************************************************/

#ifndef SRC_ENDAT22_HPP_
#define SRC_ENDAT22_HPP_

#include <types.hpp>
#include "xparameters.h"
#include "cmath"
#include "Constants.hpp"
//#include <unistd.h>
#include "sleep.h"
#include "LPFilter1.hpp"
#include "Stopwatch.hpp"

#define ENDAT_22_READ(BASEADDR)	*(volatile int*)((BASEADDR))  // Received value value plus offset.
#define ENDAT_22_WRITE(OFFSET)	*(volatile int*)(BASEADDR + OFFSET)

#define SND_REG_OFFSET   	0x00	/* Send Register */
#define RCVE_REG1_OFFSET  	0x04	/* Receive Register 1 */
#define RCVE_REG2_OFFSET  	0x0C	/* Receive Register 2 */
#define RCVE_REG3_OFFSET  	0x10	/* Receive Register 3 */
#define RCVE_REG4_OFFSET  	0x30	/* Receive Register 4 */
#define CNFG_REG1_OFFSET   	0x14	/* Configuration Register 1 */
#define CNFG_REG2_OFFSET   	0x18	/* Configuration Register 2 */
#define CNFG_REG3_OFFSET   	0x1C	/* Configuration Register 3 */
#define STATUS_REG_OFFSET   0x20	/* EnDat Message Status Register */
#define INRPT_REG_OFFSET 	0x24	/* Interrupt Status register */
#define TST_REG1_OFFSET     0x28	/* Test Register 1 */
#define TST_REG2_OFFSET     0x2C	/* Test Register 2 */
#define SW_STR_REG_OFFSET	0x38	/* Software Strobe */
#define ID_REG_OFFSET	    0x3C	/* Identification Register*/

#define CAB_PROP_TIME_OFFSET 16		/* Cable propagation time measurement */
#define MRS_CMD_OFFSET  16		/* Cable propagation time measure */
#define MRS_CODE_OFFSET 0

#define HWSTR_BIT 0
#define DU_BIT 1
#define DT_BIT 2
#define FTCLK_BIT 4
#define DWORDLEN_BIT 8
#define RESET_WIN_BIT 14
#define AUTO_RESET_BIT 15
#define CABLE_PROP_TIME_BIT 16
#define DELAY_COMP_BIT 24
#define FSYS_BIT 26
#define IC_RESET_BIT 29
#define ENDATSSI_BIT 30

#define MODE_BITS_OFFSET	24
#define PARAM_INSTR_OFFSET	8

#define SSI_MODE 1
#define ENDAT_MODE 2

#define MODE_S_POS_ADD_DATA				0x38
#define MODE_S_POS_ADD_DATA_R_MRS		0x09
#define MODE_S_POS_ADD_INFO_R_PARAMS 	0x1B
#define MODE_S_POS_ADD_INFO_PARAMS		0x24
#define MODE_S_POS_ADD_INFO_R_ERR_RST	0x2D
#define MODE_S_POS_ADD_INFO_R_TST_CMD	0x36
#define MODE_R_COMM_CMD					0x12

#define GET_POSITION 	0x98001942
#define RESET_ENCODER 	0x2A000000

#define E22_IP_CORE_ID	0xE226051B

#define POSITION_MSK_ECN425  0x01FFFFFF
#define POSITION_MSK_EQN1325 0x00001FFF
#define DWORD_LEN_ECN425 25
#define DWORD_LEN_EQN1325 13

#define E22_FIRMWARE_VERSION 0xE226051B

class EnDat22 {
public:
	EnDat22();
	virtual ~EnDat22();
	uint32_t registers[16];
	const uint32_t initialize(void);
	void getRegValues(void);
	void send(const uint8_t command);
	void sendWord(const uint32_t command);
	void resetEncoder(void);
	void NOP(void);
	void getPosition(void);
	void setfCLK(const uint32_t clkDivider);
	void resetStatusReg(void);
	void measureCablePropTime(void);
	void swStrobe(void);
	void selectMRS(const uint8_t cmd, const uint8_t mrs_code, const uint8_t mem_address);
	void getPositionE21(void);
	void e21ReadMemory(void);
	void e22ReadMemory1(uint8_t mode_cmd, uint8_t MRS_code);
	void e22ReadMemory2(uint8_t mode_cmd, uint8_t MRS_code);
	uint32_t SEND_reg = 0;
	uint32_t REC1_reg = 0;
	uint32_t REC2_reg = 0;
	uint32_t REC3_reg = 0;
	uint32_t CONF1_reg = 0;
	uint32_t CONF2_reg = 0;
	uint32_t CONF3_reg = 0;
	uint32_t STATUS_REG = 0;
	uint32_t INTR_REG = 0;
	uint32_t TST1_REG = 0;
	uint32_t TST2_REG = 0;
	uint32_t REC4_REG = 0;
	uint32_t ID_REG = 0;

	StopWatch EEPROM_time;
	fp32_t EEPROM_us;
	const uint32_t  REC1_NS_MASK 	= 0x00000001;
	const uint32_t  Error1_MASK 	= 0x00000002;
	const uint32_t  CRC_POS_MASK 	= 0x00000004;
	const uint32_t  FTYPE1_MASK 	= 0x00000008;
	const uint32_t  FTYPE2_MASK 	= 0x00000010;
	const uint32_t  MRS_ADR_MASK 	= 0x00000020;
	const uint32_t  IR6_MASK		= 0x00000040;
	const uint32_t  IR7_MASK		= 0x00000080;

	const uint32_t  REC2_NS_MASK 	= 0x00000100;
	const uint32_t  REC3_NS_MASK 	= 0x00000200;
	const uint32_t  Error2_MASK 	= 0x00000400;
	const uint32_t  CRC_ZI1_MASK 	= 0x00000800;
	const uint32_t  CRC_ZI2_MASK 	= 0x00001000;
	const uint32_t  Busy_MASK		= 0x00002000;
	const uint32_t  RM_MASK			= 0x00004000;
	const uint32_t  WRN_MASK		= 0x00008000;

	const uint32_t  Spike_MASK		= 0x00010000;
	const uint32_t  WatchDOG_MASK	= 0x00020000;
	const uint32_t  FTYPE3_MASK		= 0x00040000;
	const uint32_t  DELAY_COMP_MASK	= 0x00400000;
	const uint32_t  PROP_TIME_MEAS_BIT = 0x00800000;
	const uint32_t  READY_STR_MASK	= 0x40000000;
	const uint32_t  READY_MASK		= 0x80000000;

	const uint32_t  PROP_TIME_MEAS_MASK = 0x00FF0000;
	const uint32_t  ACK_MRS_CODE_MASK 	= 0x00FF0000;
	const uint8_t	FTCLK_BIT_OFFSET	= 4;

	enum EnDatOrderDesignation {
			e01=0x3031, e02=0x3032,
			e21 =0x3231, e22 =0x3232};

	enum StatusReturnCode {
			eINIT_OK=0x0, eIPCORE_VER_MISMATCH=0x1};

	LpFilter1    	m_SpeedFilter;
	// fclk=0.1MHz => no response from encoder
	uint32_t BASEADDR = 0;
	uint32_t OFFSET = 0;
	uint32_t X_REGISTER = 0;
	uint32_t CNFG_DEFAULT_VAL = 0x00000DF0; //c;// 0x00000D(F/6)0 ==> /*f_clk=0.1/8.33 MHz DWORD_LENGTH=13bit f_sys=64MHz EnDat=NotPermitted*/
 	uint32_t SEND_VAL = 0x07000000;
 	uint32_t CFG_2_VAL = 0x4000;
 	bool RESET_e22_if = false;
 	const fp64_t MAX_POS_ECN425  = pow(2, DWORD_LEN_ECN425);  // ECN425 = 33554432
 	const fp64_t MAX_POS_EQN1325 = pow(2, DWORD_LEN_EQN1325); // EQN1325 = 8192
 	// replace to above with one standard taken frmo encoder itself
 	const fp64_t inv_Tsamp = 1/0.000125; // Sampling period
 	int64_t position_raw_new = 0U;
 	int64_t position_raw_old = 0U;
 	int64_t position_raw_diff = 0U;


 	fp64_t position_deg_old = 0U;
 	fp64_t position_deg_new = 0U;
 	fp64_t position_rad_new = 0U;
 	fp64_t position_rad_old = 0U;
 	fp64_t position_rad_diff_old = 0U;
 	fp64_t position_rad_diff_new = 0U;
 	fp64_t omega_mech = 0U;
 	fp64_t speed_rpm = 0U;
 	fp64_t m_speed_RPM_filtered = 0U;

 	uint32_t SW_STR_COUNTER = 0U;
 	uint32_t CABLE_PROP_TIME_NO_CLKCYC = 0U;
 	uint32_t acknowledged_MRS_code = 0U;
 	uint32_t acknowledged_MRS_code2 = 0U;

 	uint32_t testValue = 0xabcd1234;
 	int flag_top = 0;

 	int32_t CAPTURE = 0xABCD1234;
 	int32_t CAPTURE2 = 0xA0B0C0D0;

 	struct status {
 		bool REC1_NS = false;
 		bool Error1 = false;
 		bool CRC_POS   = false;
 		bool FTYPE1 = false;
 		bool FTYPE2 = false;
 		bool MRSADR = false;
 		bool IR6	= false;
 		bool IR7	= false;
 		bool REC2_NS = false;
 		bool REC3_NS = false;
 		bool Error2	 = false;
 		bool CRC_ZI1 = false;
 		bool CRC_ZI2  = false;
 		bool Busy	= false;
 		bool RM	= false;
 		bool WRN	= false;
 		bool Spike	= false;
 		bool WatchDOG	= false;
 		bool FTYPE3		= false;
 		bool DELAY_COMP	= false;
 		bool PROP_TIME_MEAS = false;
 		bool READY_STR	= false;
 		bool READY	= false;

 	} status;

 	struct EEPROM_PARAM_ENCODER_MANUFACT {
 		uint32_t W4_Mask0 = 0;
 		uint32_t W5_Mask1 = 0;
 		uint32_t W6_Mask2 = 0;
 		uint32_t W7_Mask3 = 0;
 		uint32_t W8_EnDatVersion = 0;
 		uint32_t W9_MemAlloc_OEM_Params1 = 0;
 		uint32_t W10_MemAlloc_OEM_Params2 = 0;
 		uint32_t W11_MemAlloc_for_Comp_Values1  = 0;
 		uint32_t W12_MemAlloc_for_Comp_Values2 = 0;
 		uint32_t W13_Clock_Pulses_Position_Value = 0;
 		uint32_t W14_EncoderModel = 0;
 		uint32_t W15_Signal_period_perRev_IncrSigs1 = 0;

 		uint32_t W16_Signal_period_perRev_IncrSigs2 = 0;
 		uint32_t W17_Distinguisable_Revolutions = 0;
 		uint32_t W18_Incr_of_Ref_Marks = 0;
 		uint32_t W19_Pos_of_1st_Reference_Mark = 0;
 		uint32_t W20_Measuring_StepsPerRev1 = 0;
 		uint32_t W20_Measuring_StepsPerRev2 = 0;
 		uint32_t W22_DatumShift_From_Manufacturer1 = 0;
 		uint32_t W23_DatumShift_From_Manufacturer2 = 0;
 		uint32_t W24_ID1 = 0;
 		uint32_t W25_ID2 = 0;
 		uint32_t W26_ID3 = 0;
 		uint32_t W27_SN1 = 0;
 		uint32_t W28_SN2 = 0;
 		uint32_t W29_SN3 = 0;
 		uint32_t W30_Direction_Rotation = 0;
 		uint32_t W31_Status_Commissioning_Diagnosi = 0;

 		uint32_t W32_Max_Shaft_Speed = 0;
 		uint32_t W33_Accuracy_Area1 = 0;
 		uint32_t W34_Accuracy_Area2 = 0;
 		uint32_t W35_Support_of_Err1_Msg = 0;
 		uint32_t W36_Support_of_Warnings = 0;
 		uint32_t W37_EnDat_Command_Set = 0;
 		uint32_t W38_Measuring_Length = 0;
 		uint32_t W39_Max_Calc_Time_Position = 0;
 		uint32_t W40_EnDat_Order_Designation = 0;
 		uint32_t W41_OEM_Data1 = 0;
 		uint32_t W42_OEM_Data2  = 0;
 		uint32_t W43_OEM_Data3  = 0;
 		uint32_t W44_OEM_Data4  = 0;
 		uint32_t W45_OEM_Data5  = 0;
 		uint32_t W46_OEM_Data6  = 0;
 		uint32_t W47_CHECKSUM = 0;
 	} EEPROM_MANUFACTURER;
};

#endif /* SRC_ENDAT22_HPP_ */
