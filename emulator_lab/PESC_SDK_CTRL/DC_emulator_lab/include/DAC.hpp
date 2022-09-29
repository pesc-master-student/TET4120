
/************************************************************************************

::FILENAME: DAC.hpp
::DESCRIPTION: Software driver for Analog Devices AD5447 digital-to-analog converter.
 
 
                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
                   Power Electronic Systems and Components
              Department of Electric Power Engineering, NTNU 2019

 
::SIGNAL FLOW/PIPELINE::
# -------- HARDWARE --------- # - FPGA/FIRMWARE - # -------- CPU/SOFTWARE -------- #
 ____________     __________     ___________		  	  ___________________
|   ANALOG   |	 |  AD5447  |   |    DAC    |  	         |  	  DAC	     |
|   OUTPUT   |<==|   CHIP   |<==|  IP-CORE  |<==AXI-BUS==|  SOFTWARE DRIVER  |
|____________|	 |__________|   |___________|	         |___________________|
                                     /\
                                 ____||____
                                |  ARRAY   |
                                |  RESIZE  |<== FIRMWARE
                                |__________|    SIGNALS

::HARDWARE DEPENDENCIES::
- Analog Devices AD5447 (12-bit/20 MSPS, 2 channel, serial LVDS 1.8V)

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'da_converter_interface_v1_00_a'
- SINTEF Energy IP-Core 'array_resize_v1_00_a'

           IP-Core Configuration Property                 Recommended Setting
          =====================================================================
           Number of signal sources                       32
           Width in                                       16
           Use scaling                                    1
           Scale shifts                                   8
           Config reg default value                       0x00000000
           Clock pulses CS low                            2
           DA converter width                             12
           Number of channels                             4
           C S Axi Addr Width                             6
           C S Axi Data Width                             32
           C S Axi Aclk Freq Hz                           100000000
          =====================================================================

::SOFTWARE DEPENDENCIES::
- Register definitions in 'DA_converter_registers.h'
 
::MINIMUM SOFTWARE SETUP::


::MINIMUM FIRMWARE SETUP::



*************************************************************************************
::FUNCTIONAL DESCRIPTION::

OFFSET--12BIT---[ * 2^SCALE_SHIFTS]
                                  |
SCALE--18BIT-------\            44BIT                                    2048
                    \             |                                       |
MUX--DATA--25BIT--[ * ]--44BIT--[ + ]--[ / 2^SCALE_SHIFTS ]--44/12BIT---[ + ]---->


::SIGNAL SELECTION::
DAC IP-Core features two levels of signal multiplexing. MUX Level 1 receives bundle
of signals from surrounding firmware, select/route to next stage. Level 2 MUX toggles
between MUX 1 and input signal from software layer.

::INPUT REGISTERS::
Selected data signal is trimmed by scaling and biasing using Xilinx DSP48 primitive.
Word size of these three registers is constrained by DSP48 limitations. Violating
specified dynamic range causes overflow. Inputs truncated to word sizes given below.

        Data Register           Offset Register      Scaling Register
        =====================   ===============      ================
WIDTH   25                      12                   18
MAX    +16777215 = 0x00FFFFFF  +2047 = 0x07FF       +131071 = 0x1FFFF
MIN    -16777216 = 0xFF000000  -2048 = 0xF800       -131072 = 0FE0000

::OUTPUT REGISTER::
DSP48 output stage is 44-bit and accommodates full-scale inputs without overflowing.
Output from DSP48 core is truncated into 12-bit format and if necessary clamped to
max/minimum limits so to fit the DAC IP-Core output register. A +2048 fixed offset
is finally added to enable 4Q bipolar DAC operation.

Z = 2048 + [(INPUT_SIG * SCALING) + 2^SCALE_SHIFTS * OFFSET] / 2^SCALE_SHIFTS

::ANALOG OUTPUT VOLTAGE::
Measurable DAC output signal is given by equation:
V_out = V_ref * (Z / 2048) + V_ref
      = 2.50 * (Z / 2048) + 2.50
      = 1.22E-3 * Z - 2.50

Voltage  Output Register (Z)
=======  ===================
+2.50 V  4095 = 0x0FFF
 0.00 V  2048 = 0x0800
-2.50 V  0000 = 0x0000

*************************************************************************************
::MODIFICATION HISTORY::

Ver  Author                      Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 aravinda.perera@ntnu.no     11.04.2019 Original code release.
2.00 thomas.haugan@ntnu.no       20.11.2019 Added comments/documentation.Code cleanup
                                            and adaption for NTNU PESC FW/SW platform.
                                            Added memory map.
2.10 thomas.haugan@ntnu.no       27.11.2019 Finished testing and demystifying the DAC
                                            IP-Core signal routing and processing.
                                            Added more detailed and correct description
                                            based on IP-Core VHDL design files.
                                            Corrected memory map explanation. Added
                                            signal flow graphics.
2.20 thomas.haugan@ntnu.no       02.12.2019 Finalized implementation code, except
                                            initialize(..) and updateDAC(..).
*************************************************************************************
::IP-CORE MEMORY MAP::

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation dependent while register offsets are static.

Offset  Register Name                             Access Register Content (32-bit)
------  ---------------------------------------   ------ ----------------------
DAC SIGNAL INPUT
NOTE: Register do not mirror DAC output as effect of gain/bias are not reflected.
0x0000: DA_OUTPUT_A                               RW     DAC Signal Input Channel A
0x0004: DA_OUTPUT_B                               RW     DAC Signal Input Channel B
0x0008: DA_OUTPUT_C                               RW     DAC Signal Input Channel C
0x000C: DA_OUTPUT_D                               RW     DAC Signal Input Channel D

IP-CORE CONFIGURATION REGISTER
0x0010: DA_CONVERTER_CONFIG_REG                   RW     MUX2 Select/route FW or SW
 
DAC FIRMWARE SIGNAL SOURCE SELECTOR
0x0014: DA_CONVERTER_SIGNAL_SOURCE_SELECTOR       RW     MUX1 Select FW signal
 
DAC OUTPUT SCALING
0x0018: DA_OUTPUT_SCALE(0)                        RW     Amplification Factor CH A
0x001C: DA_OUTPUT_SCALE(1)                        RW     Amplification Factor CH B
0x0020: DA_OUTPUT_SCALE(2)                        RW     Amplification Factor CH C
0x0024: DA_OUTPUT_SCALE(3)                        RW     Amplification Factor CH D
 
DAC OUTPUT BIASING
0x0028: DA_OUTPUT_OFFSET(0)                       RW     Offset CH A
0x002C: DA_OUTPUT_OFFSET(1)                       RW     Offset CH B
0x0030: DA_OUTPUT_OFFSET(2)                       RW     Offset CH C
0x0034: DA_OUTPUT_OFFSET(3)                       RW     Offset CH D

*************************************************************************************
::FIRMWARE SIGNAL MAPPING::

MUX1 is an 8-bit multiplexer capable selecting from maximum 256 different firmware
signals. Note that if using Resize_array block in front of DAC IP-Core this block will
provide a prescaling effect.
 
		IF (WIDTH_IN < WIDTH_OUT)
		   Amplification = 2^(WIDTH_OUT - WIDTH_IN)
		ELSE
		   Attenuation = 2^(WIDTH_IN - WIDTH_OUT)

Scaling and mapping of firmware signals are of course implementation dependent.

 GROUP 1 : Amplification 2^(16-13) = 2^3   = 8
 GROUP 2 : Attenuation   2^(24-16) = 2^256 = 1/256
 GROUP 3 : Amplification 2^(16-13) = 2^3   = 8
 GROUP 4 : Amplification 2^(16-14) = 2^2   = 4
 GROUP 5 : Amplification 2^(16-13) = 2^3   = 8

 ==============================================================
 CH SIGNAL DESIGNATION
 -- -------------  GROUP 1  -----------------------------------
 00 FW Converter 1 - LEM Current sensor Phase A_LP_Filter
 01 FW Converter 1 - LEM Current sensor Phase B_LP_Filter
 02 FW Converter 1 - LEM Current sensor Phase C_LP_Filter
 03 FW Converter 1 - LEM Voltage sensor DC-Link Voltage_LP_Filter
 --
 04 FW Converter 2 - LEM Current sensor Phase A_LP_Filter
 05 FW Converter 2 - LEM Current sensor Phase B_LP_Filter
 06 FW Converter 2 - LEM Current sensor Phase C_LP_Filter
 07 FW Converter 2 - LEM Voltage sensor DC-Link Voltage_LP_Filter
 -- -------------  GROUP 2  -----------------------------------
 08 FW Converter 1 - LEM Current sensor Phase A_MA_Filter
 09 FW Converter 1 - LEM Current sensor Phase B_MA_Filter
 10 FW Converter 1 - LEM Current sensor Phase C_MA_Filter
 11 FW Converter 1 - LEM Voltage sensor DC-Link Voltage_MA_Filter
 12 FW Converter 1 - Integrator sample counter
 --
 13 FW Converter 2 - LEM Current sensor Phase A_MA_Filter
 14 FW Converter 2 - LEM Current sensor Phase B_MA_Filter
 15 FW Converter 2 - LEM Current sensor Phase C_MA_Filter
 16 FW Converter 2 - LEM Voltage sensor DC-Link Voltage_MA_Filter
 17 FW Converter 2 - Integrator sample counter
 -- -------------  GROUP 3  -----------------------------------
 18 FW Converter 1 - LEM Current sensor Phase A_LP_Filter_Sync_sampling
 19 FW Converter 1 - LEM Current sensor Phase B_LP_Filter_Sync_sampling
 20 FW Converter 1 - LEM Current sensor Phase C_LP_Filter_Sync_sampling
 21 FW Converter 1 - LEM Voltage sensor DC-Link Voltage_LP_Filter_Sync_sampling
 --
 22 FW Converter 2 - LEM Current sensor Phase A_LP_Filter_Sync_sampling
 23 FW Converter 2 - LEM Current sensor Phase B_LP_Filter_Sync_sampling
 24 FW Converter 2 - LEM Current sensor Phase C_LP_Filter_Sync_sampling
 25 FW Converter 2 - LEM Voltage sensor DC-Link Voltage_LP_Filter_Sync_sampling
 -- -------------  GROUP 4  -----------------------------------
 26 FW Converter 1 - Hysteresis current regulator error Phase A
 27 FW Converter 1 - Hysteresis current regulator error Phase B
 28 FW Converter 1 - Hysteresis current regulator error Phase C
 --
 29 FW Converter 2 - Hysteresis current regulator error Phase A
 30 FW Converter 2 - Hysteresis current regulator error Phase B
 31 FW Converter 2 - Hysteresis current regulator error Phase C
 -- -------------  GROUP 5  -----------------------------------
 32 FW Converter 1 - ADC Channel A
 33 FW Converter 1 - ADC Channel B
 34 FW Converter 1 - ADC Channel C
 35 FW Converter 1 - ADC Channel D
 --
 36 FW Converter 2 - ADC Channel E
 37 FW Converter 2 - ADC Channel F
 38 FW Converter 2 - ADC Channel G
 39 FW Converter 2 - ADC Channel H
 -- -------------  GROUP 6  -----------------------------------
 40 FW DC-Drive Emulator / I_a / I_a
 41 FW DC-Drive Emulator / n (per-unit speed) / I_b
 42 FW DC-Drive Emulator / theta / I_c
 43 FW DC-Drive Emulator / Ua0_1 / I_d
 44 FW DC-Drive Emulator / Uab_1 / I_q
 45 FW DC-Drive Emulator / Uab_2 / speed
 46 FW DC-Drive Emulator / Ub0_1 / theta
 ==============================================================


*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
 - Complete documentation in this .hpp file.
 - Test and verify code on Picozed/WatchView/Oscilloscope
 - Differentiate SW (needs scaling and offset) from FW (already signal conditioned)
*************************************************************************************/

#pragma once

#include "DA_converter_registers.h"
#include "basis_io_picozed7030_based_processor_board.h"
#include <math.h>


//--------------Scale Definitions-------------------
// Applies only for software? Because FW is implementation
// dependent pre-scaling.
#define DA_MILLI_FULL_SCALE			0.0025
#define DA_CENTI_FULL_SCALE			0.025
#define DA_DECI_FULL_SCALE			0.25
#define DA_UNIT_FULL_SCALE			2.5
#define DA_DECA_FULL_SCALE			25
#define DA_HECTA_FULL_SCALE  		250
#define DA_KILO_FULL_SCALE 			2500
#define DA_TENKILO_FULL_SCALE		25000


class DAC {
public:
	static const int m_SOURCE_COUNT = XPAR_DA_CONVERTER_NUMBER_OF_SIGNAL_SOURCES;
	static const int m_CH_OUT_COUNT = XPAR_DA_CONVERTER_NUMBER_OF_CHANNELS;
	int const m_MAX_INPUT = pow(2, XPAR_DA_CONVERTER_WIDTH_IN - 1);
	int const m_MAX_GAIN = pow(2, 18 - 1);
	int const m_MAX_OFFSET = pow(2, 12 - 1);
    enum OUT_PHY_CH { CH_A = 0, CH_B = 1, CH_C = 2, CH_D = 3 };

	// Firmware input mux is 8-bit, thus 2^8 = 256 distinguishable signals
	// Convenience enumeration for input select CH00-CH149 (150 channels).
	// check if enum e_Mux1 {....} MUX1 works (is available in WV)
	enum e_Mux {
		CH00 = 0, CH10 = 10, CH20 = 20, CH30 = 30, CH40 = 40,
		CH01 = 1, CH11 = 11, CH21 = 21, CH31 = 31, CH41 = 41,
		CH02 = 2, CH12 = 12, CH22 = 22, CH32 = 32, CH42 = 42,
		CH03 = 3, CH13 = 13, CH23 = 23, CH33 = 33, CH43 = 43,
		CH04 = 4, CH14 = 14, CH24 = 24, CH34 = 34, CH44 = 44,
		CH05 = 5, CH15 = 15, CH25 = 25, CH35 = 35, CH45 = 45,
		CH06 = 6, CH16 = 16, CH26 = 26, CH36 = 36, CH46 = 46,
		CH07 = 7, CH17 = 17, CH27 = 27, CH37 = 37, CH47 = 47,
		CH08 = 8, CH18 = 18, CH28 = 28, CH38 = 38, CH48 = 48,
		CH09 = 9, CH19 = 19, CH29 = 29, CH39 = 39, CH49 = 49,

		CH50 = 50, CH60 = 60, CH70 = 70, CH80 = 80, CH90 = 90,
		CH51 = 51, CH61 = 61, CH71 = 71, CH81 = 81, CH91 = 91,
		CH52 = 52, CH62 = 62, CH72 = 72, CH82 = 82, CH92 = 92,
		CH53 = 53, CH63 = 63, CH73 = 73, CH83 = 83, CH93 = 93,
		CH54 = 54, CH64 = 64, CH74 = 74, CH84 = 84, CH94 = 94,
		CH55 = 55, CH65 = 65, CH75 = 75, CH85 = 85, CH95 = 95,
		CH56 = 56, CH66 = 66, CH76 = 76, CH86 = 86, CH96 = 96,
		CH57 = 57, CH67 = 67, CH77 = 77, CH87 = 87, CH97 = 97,
		CH58 = 58, CH68 = 68, CH78 = 78, CH88 = 88, CH98 = 98,
		CH59 = 59, CH69 = 69, CH79 = 79, CH89 = 89, CH99 = 99,

		CH100 = 100, CH110 = 110, CH120 = 120, CH130 = 130, CH140 = 140,
		CH101 = 101, CH111 = 111, CH121 = 121, CH131 = 131, CH141 = 141,
		CH102 = 102, CH112 = 112, CH122 = 122, CH132 = 132, CH142 = 142,
		CH103 = 103, CH113 = 113, CH123 = 123, CH133 = 133, CH143 = 143,
		CH104 = 104, CH114 = 114, CH124 = 124, CH134 = 134, CH144 = 144,
		CH105 = 105, CH115 = 115, CH125 = 125, CH135 = 135, CH145 = 145,
		CH106 = 106, CH116 = 116, CH126 = 126, CH136 = 136, CH146 = 146,
		CH107 = 107, CH117 = 117, CH127 = 127, CH137 = 137, CH147 = 147,
		CH108 = 108, CH118 = 118, CH128 = 128, CH138 = 138, CH148 = 148,
		CH109 = 109, CH119 = 119, CH129 = 129, CH139 = 139, CH149 = 149,

		SOFTWARE = 300
	} MUX1;

	enum e_MuxL2 {PS = 0, PL = 1};
	int muxL1;
	int muxL2;

	struct CH_SETUP {
		e_Mux source_selector;
		OUT_PHY_CH output;
		float signal;
		float gain;
		float offset;
	};

	struct CH_SETUP m_CH[m_CH_OUT_COUNT];


/************************************************************************************
#Fetch DAC IP-Core firmware base address#

Get base address of the 'da_converter_interface_v1_00_a' IP-Core from hardware
definition generated by Vivado.
This function must be run during software initialization and before calling
any other functions of this class.

@param: void
@return: void
@note: None
************************************************************************************/
void getFWAddress(void); // OK

/************************************************************************************
#Function Head Line - TODO#

...Description...

generates signal selection list and calls config routine
return error code if init did not work
do some write and read-back tests to check all registers, then reset register
and put IP Core into a predefined standard state.
Health check of registers, use unique values for each register. Values that does
not cause unintentional damage or behavior to converter

generates signal selection list and calls config routine
if runTest = true then exercise all registers write/read back

SW_SIG_A..D are public structs of DAC.hpp
init SW_SIG_A
DAC init function must return ERROR_CODE if could not find baseadress or
init the DAC object properly. This will in turn guide the application FSM

@param: void
@return: void
@note: None
************************************************************************************/
void initialize(void);

/************************************************************************************
#Function Head Line - TODO#

...Description...

SetValue Called regularly from main interrupt. Writes software values to DA ports
This is for writing signals from software domain to DAC output. Note the config_register
must be set to route from register rather than FW.

@param: void
@return: void
@note: None
************************************************************************************/
void updateDAC(CH_SETUP *ch_A, CH_SETUP *ch_B, CH_SETUP *ch_C, CH_SETUP *ch_D);

/************************************************************************************
#Help function route selected input source to selected DAC physical output channel#

...Description...

all registers are set to preset state
Note: skipped 2nd argument, thus indirectly defaults to preset state values

@param: void
@return: void
@note: None
************************************************************************************/
void setMuxL1(OUT_PHY_CH srcSelector, e_Mux physOutCh);

/************************************************************************************
#Function Head Line - TODO#

...Description...

all registers are set to preset state
Note: skipped 2nd argument, thus indirectly defaults to preset state values

@param: void
@return: void
@note: None
************************************************************************************/
void resetChannel(OUT_PHY_CH output_channel);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Every DAC output channel can select up to 2^8 = 256 different input signals.
The internal MUX is designed for this, the firmware design must be routed and
support that amount of signals.

@param: void
@return: void
@note: None
************************************************************************************/
void updateDAC(CH_SETUP *channel);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Note: skipped 2nd argument, thus defaults to preset state values

##check if this overwrites FW input or if you
  first need to reset respective config register bit
##check if this SW signal is then scaled and offset within DAC IP-Core

Maybe it is better to call DA_OUTPUT(channel) register the
input register because it reflects unchanged input, does not
show effect of bias+scaling

Check if inf/float parameter with corresponding argument
will be the same, i.e., that fractional is truncated.
262016 equals to half-scale positive (+1.25 V)

@param: void
@return: void
@note: None
************************************************************************************/
void setInput(OUT_PHY_CH ch, int signal = 262016);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Test this and verify: is it really output register Z (including offset/gain)
or is it unaffected input. Or is it software input reg, so you cannot really
read of the muxed firmware signal?

@param: void
@return: void
@note: None
************************************************************************************/
void getAllOutput(void);

/************************************************************************************
#Function Head Line - TODO#

...Description...

@param: void
@return: void
@note: None
************************************************************************************/
void getMuxL1(void);

/************************************************************************************
#Function Head Line - TODO#

...Description...

@param: void
@return: void
@note: None
************************************************************************************/
void getMuxL2(void);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Level-2 multiplexer, toggles between FW and SW signal source.
This is referred to as configuration register in skriften.
Tested and confirmed it toggles config
register bits 0 to 3. Setting bit/channel
true/false selects FW/SW respectively.

When selecting SW this will get prioritized
above firmware signals sources. Consequently
resetting bit, the FW MUX will be disabled for that channel.

@param: void
@return: void
@note: None
************************************************************************************/
void setMuxL2(OUT_PHY_CH channel, e_MuxL2 state);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Based on known execution rate/interrupt frequency use an internal counter
and increment each interrupt + also measure and log maximum value over 20-60 ms.
Then scale the signal with 10% headroom.
If SW = measure the signal directly. FW must first sample signal from IP-Core register
In firmware take to account the pre-scaling in resize array IP-Block

@param: void
@return: void
@note: None
************************************************************************************/
void autoScale(int signalSelector, int channel);

/************************************************************************************
#Adjust offset for specified DAC IP-Core output channel#

Use inspiration from ADC class

Note: skipped 2nd argument, thus defaults to preset state values

@param: void
@return: void
@note: None
************************************************************************************/
void setOffset(OUT_PHY_CH channel, int offset = 0);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Note: skipped 2nd argument, thus defaults to preset state values

@param: void
@return: void
@note: None
************************************************************************************/
void selectInput(OUT_PHY_CH channel, e_Mux select = SOFTWARE);

/************************************************************************************
#Fetch offset compensation values from DAC IP-Core#

Read-back function to update SW with bias factors for CHA,B,C,D
Expected valid range is [-m_MAX_OFFSET, m_MAX_OFFSET].
How does it translate to PHY dac volt output

@param: void
@return: void
@note: None
************************************************************************************/
void getAllOffset(void); // OK
    
/************************************************************************************
#Function Head Line - TODO#

...Description...

Scaling works on both SW and FW signals
Signed integer accepted
Check valid gain applying inly for input
Test if combined max offset, gain and input together
overflows of saturates the DSP48?
Read-back into class internal scratch register

Scaling is set to default 1 if arg not provided.
Note: skipped 2nd argument, thus defaults to preset state value (scaling=1)

@param: void
@return: void
@note: None
************************************************************************************/
void setScaling(OUT_PHY_CH channel, int scaling = 1);

/************************************************************************************
#Function Head Line - TODO#

...Description...

Read-back function to update SW with scaling factors. Strictly speaking
only amplification allowed.

@param: void
@return: void
@note: None
************************************************************************************/
void getAllScaling(void);

private:
    int m_base_address;
    int m_ch_input[m_CH_OUT_COUNT];
    int m_ch_gain[m_CH_OUT_COUNT];
    int m_ch_offset[m_CH_OUT_COUNT];


};


