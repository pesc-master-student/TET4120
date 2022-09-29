
/************************************************************************************
::FILENAME: Filter.hpp
::DESCRIPTION: Software driver for digital filter implemented on FPGA logic.


                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
             Department of Electric Power Engineering, NTNU 2020



::SIGNAL FLOW/PIPELINE::
# ------------ FPGA/FIRMWARE ------------- # -------- CPU/SOFTWARE -------- #
			 	   	     ____________              ___________________
FILTER_FW_SIGNAL_IN  ==>|   FILTER   |  		  |   DIGITAL FILTER  |
                        |  IP-CORE   |==AXI-BUS==>|  SOFTWARE DRIVER  |
FILTER_FW_SIGNAL_OUT <==|____________|            |___________________|
				

::HARDWARE DEPENDENCIES::
- None

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'filter_block_v1_0'

		   LOW-PASS FILTER
		   IP-Core Configuration Property			 	Recommended Setting
		  ==================================================================
		   Operation Mode      							1
		   Number Of Channels							4
		   Signal In Width								13
		   Signal Out Width								13
		   Multiplier Width								25
		   Accumulator Additional Bits					0
		   Input Fractional Bits						0
		   Input Source Config							2
		   Scaling										1
		   Common Parameters							0
		   Accumulator Write							1
     	  ==================================================================
 
           MOVING-AVERAGE FILTER
           IP-Core Configuration Property               Recommended Setting
          ==================================================================
           Operation Mode                               0
           Number Of Channels                           5
           Signal In Width                              13
           Signal Out Width                             24
           Multiplier Width                             25
           Accumulator Additional Bits                  0
           Input Fractional Bits                        0
           Input Source Config                          2
           Scaling                                      1
           Common Parameters                            0
           Accumulator Write                            1
          ==================================================================
 
LOW-PASS FILTER
* Operation Mode equals 1 meaning a first-order low-pass filter is implemented.
* Four input channels are needed (3 phase currents and 1 DC-link voltage).
* Signal In width equals 13 bits. The ADC The is sufficient while the AD-converter is
  12 bit and in additional signed value. The max value from AD-converter is thus 2047,
  while 13 bit can represent [-4096,4095].
* Signal Out Width is also 13 bit, filter gain is 1 in the pass-region .
* The Multiplicator is 25 bit. This means that the Accumulator becomes 25+13=38 bits.
* Input Source Config equal 2 means that the IP-core can be configured by hardware
 or via the AXI bus.
* Common Parameters equal 0 means that we can have different filter time con-stants
  for the different channels.
* Accumulator Write equals 1 (TRUE), this allows the write data directly means that
  we can write values to the accumulator from the processor via AXI-bus.

MOVING-AVERAGE FILTER
* Select register width equal 24 as for the moving integral output.
* Number of channels equal 5, i.e. 3 for currents, one for voltage and one for number
  of measurements within a sampling interval Tsamp of the controller.

 
::SOFTWARE DEPENDENCIES::
- Register definitions in 'filter_block_registers.h'

::MINIMUM SOFTWARE SETUP::
- getFWaddress(...);
- initialize(...);
- tuneLPfilterCutFreq(...);
- tuneMAfilterScaleMode(...);
- getInputSamples(...);
- getOutSamples(...);

::MINIMUM FIRMWARE SETUP::
- SINTEF Energy IP-Core 'filter_block_v1_0'

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author                      Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 kjell.ljokelsoy@sintef.no   01.01.2008 Original code release.
2.00 thomas.haugan@ntnu.no       04.11.2019 Added documentation and adding auxiliary
                                            functions. Testing filter in conjunction
                                            with ADC SW/FW.
2.10 thomas.haugan@ntnu.no   	 20.11.2019 Reworked documentation to same format as
										    in ADC.hpp.
2.20 thomas.haugan@ntnu.no       14.02.2020 Added function tuneMAfilterScaleMode(..)
                                            for tuning MA-filter gain. Completed
                                            testing in lab with real converter system.
2.30 thomas.haugan@ntnu.no       03.03.2020 Completed documentation.
*************************************************************************************
IP-CORE MEMORY MAP:


BASE_ADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address register offsets are dynamic.

Offset  Register Name                            Access Register Content (32-bit)
------  ---------------------------------------  ------ ----------------------
FILTER_CONFIGURATION_REGISTER
0x0000  FILTER_CONFIG_REG(BASE_ADDR)                    Bit-Mapping (LSB = 0)
                                                 RW     0: ACCUMULATOR STATE
                                                           Inhibit updates = TRUE
                                                           Normal mode = FALSE
                                                 RW     1: LOAD ACCUMULATOR INIT DATA
                                                           Normal = FALSE
                                                           Load accumulator = TRUE
                                                 RW     2: INPUT SIGNAL MULTIPLEXER
                                                           Signal source 0 = PL
                                                           Signal source 1 = PS
                                                     3-31: UNDEFINED
                                                
FILTER_OUTPUT_REGISTER
0x0004  FILTER_OUT_REG(BASE_ADDR, 0)             RW     Channel 0 - Filter Output # Current Phase A
0x0008  FILTER_OUT_REG(BASE_ADDR, 1)             RW     Channel 1 - Filter Output # Current Phase B
0x000C  FILTER_OUT_REG(BASE_ADDR, 2)             RW     Channel 2 - Filter Output # Current Phase C
0x0010  FILTER_OUT_REG(BASE_ADDR, 3)             RW     Channel 3 - Filter Output # DC-Link Voltage

 
Z = FILTER_MULTIPLIER_REG(BASE_ADDR, NO_OF_CH, CH)
0x0014  Z(BASE_ADDR, NO_OF_CH, 0)                RW    DSP48_GAIN - Channel 0
0x0018  Z(BASE_ADDR, NO_OF_CH, 1)                RW    DSP48_GAIN - Channel 1
0x001C  Z(BASE_ADDR, NO_OF_CH, 2)                RW    DSP48_GAIN - Channel 2
0x0020  Z(BASE_ADDR, NO_OF_CH, 3)                RW    DSP48_GAIN - Channel 3
 // FILTER_MULTIPLIER(BASE_ADDR,0) is used when COMMON_PARAMETERS is set to 1.
 // Note! Must use full width write, not bytewise write.
 // Automatically set to 1 when when SCALING is set to fixed.
 // FILTER_MULTIPLIER(b,n) = tf / (ts * 2**(FILTER_NUMBER_OF_SHIFTS)+1)
 
 
X = FILTER_NUMBER_OF_SHIFTS_REG(BASE_ADDR, NO_OF_CH, CH)
0x0024  X(BASE_ADDR, NO_OF_CH, 0)                RW    SHIFT - Channel 0
0x0028  X(BASE_ADDR, NO_OF_CH, 1)                RW    SHIFT - Channel 1
0x002C  X(BASE_ADDR, NO_OF_CH, 2)                RW    SHIFT - Channel 2
0x0030  X(BASE_ADDR, NO_OF_CH, 3)                RW    SHIFT - Channel 3
 //FILTER_NUMBER_OF_SHIFTS(BASE_ADDR,0) is used when COMMON_PARAMETERS is set to 1.
 //Number of shifts from accumlator to input. (output is shifted up and down according to the difference between input and output width.)
 //Automatically set to max value,(MULTIPLIER_WIDTH + ACCUMULATOR_ADDITIONAL_BITS-1) when SCALING is set to fixed or multiplier.
 
Y = FILTER_IN_REG(BASE_ADDR,NUMBER_OF_CHANNELS,n)
0x0034  Y(BASE_ADDR,NUMBER_OF_CHANNELS, 0)        RW    Filter Input #  - Channel 0
0x0038  Y(BASE_ADDR,NUMBER_OF_CHANNELS, 1)        RW    Filter Input #  - Channel 1
0x003C  Y(BASE_ADDR,NUMBER_OF_CHANNELS, 2)        RW    Filter Input #  - Channel 2
0x0040  Y(BASE_ADDR,NUMBER_OF_CHANNELS, 3)        RW    Filter Input #  - Channel 3
 
*************************************************************************************
::FUNCTIONAL DESCRIPTION::

  OFFSET--12BIT---[ * 2^SCALE_SHIFTS]
                                  |
SCALE--18BIT-------\            44BIT                                    2048
                    \             |                                       |
MUX--DATA--25BIT--[ * ]--44BIT--[ + ]--[ / 2^SCALE_SHIFTS ]--44/12BIT---[ + ]---->

*************************************************************************************
::DSP48E DESIGN PRIMITIVE::
 
 
 
*************************************************************************************
::SIGNAL SELECTION::
DAC IP-Core features two levels of signal multiplexing. MUX Level 1 receives bundle
of signals from surrounding firmware, select/route to next stage. Level 2 MUX toggles
between MUX 1 and input signal from software layer.



::INPUT REGISTERS::
 Input stage features a multiplexer that selects filter input signal either from
 processing system (PS) or programmable logic (PL). PS can then use the firmware
filter for hardware-accelerated computation. It can also be used for initializing
 the filter with an initial value from software. Or used if need to provide filter
 with input test data not readily accessible from elswhere in firmware layer (ADC).
 
 Maximum input range is 2^11 as we assume ADC will be properly offset corrected
 and the ADC-chip will always stream 12-bit data, so the dynamic range is expected
 in range [-2^11 - 1, 2^11]. Values outside this range we cannot guarantee not to have
 overflow.
 
 uint32_t INPUT_MAX_RANGE = pow(2, 11);

 uint32_t DSP48_OUTPUT_LSHIFT = pow(2, 1);
 
 
*************************************************************************************
::OUTPUT REGISTER::
Which signals are sampled and when plays a crucial role for scaling to add upp.
 
*************************************************************************************
::MOVING-AVERAGE FILTER::
 - The scaling strategies are based on the 13-but number system on firmware layer.
- Scaling
 
 Algorithm:
 
 DSP48_OUT = (DSP48_INPUT * DSP48_MULTIPLY * 2^Z * #SAMPLES) / (2^Y)
 
Gain-strategies
 - What are the limitations and minimum requiresments for the filter to output
   reasonable and undistorted data (avoid overflow)?
 - Explain what properties we want to achieve?
 

DSP48_ACCUMULATOR_MAX = pow(2, 36)
 = Top of the accumulator is taken out and used as filter output. The output word
 is based on slicing out bits [37...13] from DSP48 (after << 2). Considering 2's
 complement, bit 37 becomes the sign bit while bits 36...0 are the data/magnitude bits.
 Maximum accumulator magnitude is 2^35 = 3.435974E+10, a rather large accumulator
 however does not mean it cannot overflow. Combination large gain factor and input signal
 and #samples will make it overflow, so precaution needed.
 
  Minimum accumulator magnitude is 2^12 because bit 16 must be high and will be
 reorganized into bit 0 in the new sliced output. Bit 16 = HIGH => Output = 1

     Binary                  Hexadecimal          Decimal
     =====================   ===============      ================
MIN  0b0001_0000_0000_0000   0x1000               +4096      (positive number) what about negative 2s complements
MAX                          0x000F_FFFF_FFFF     +68719476735

 
 
 

 uint32_t DSP48_OUTPUT_RSHIFT_SLICE = pow(2, 14);

 samples_per_IRQ = static_cast<uint32_t>(AD_RAW_SAMPLE_RATE * fmax(T_samp, 0));

 minimum_DSP48_gain = static_cast<uint32_t>( DSP48_OUTPUT_RSHIFT_SLICE / (1 * samples_per_IRQ) );
 maximum_DSP48_gain = static_cast<uint32_t>( DSP48_ACCUMULATOR_MAX / (2048 * samples_per_IRQ) );
 average_DSP48_gain = static_cast<uint32_t>( (minimum_DSP48_gain + maximum_DSP48_gain) / 2 );

 if(mode == UNITY_GAIN){
     float temp = ceil( DSP48_OUTPUT_RSHIFT_SLICE / (DSP48_OUTPUT_LSHIFT*samples_per_IRQ) );
     temp = fmax(temp, minimum_DSP48_gain);
     DSP48_gain = static_cast<uint32_t>(temp);
 }
 else if(mode == MID_RESOLUTION)
     DSP48_gain = static_cast<uint32_t>( (0.50 * DSP48_ACCUMULATOR_MAX) / (INPUT_MAX_RANGE * samples_per_IRQ) );

 else if(mode == MAX_RESOLUTION)
     DSP48_gain = static_cast<uint32_t>( (0.90 * DSP48_ACCUMULATOR_MAX) / (INPUT_MAX_RANGE * samples_per_IRQ) );

 for(int channel = 0; channel < (m_CH_COUNT-1); channel++){
     FILTER_MULTIPLIER_REG(m_base_address, m_CH_COUNT, channel) = DSP48_gain;
 }

 FILTER_MULTIPLIER_REG(m_base_address, m_CH_COUNT, m_CH_COUNT - 1) = pow(2, 13);

 INPUT_2_OUTPUT_RATIO = pow(2, 14) / (DSP48_gain * pow(2, 1) );
 return INPUT_2_OUTPUT_RATIO;
 
 
 RATIO OF CLOCKS
 - Data rate of firmware filter and sample rate is 20 MHz
 - Rate of PWM is 4 kHz, these ratios decide the #samples
 
 
*************************************************************************************
::LOW-PASS FILTER::

 
 
*************************************************************************************
NOTIFICATONS:   None
*************************************************************************************
TODO:
- Loading values into filter output register
   * Force-write from software, check if value stays in register or quickly becomes
     overwritten by firmware data stream (use WView and DAC).
- MA-filter gain strategies
   * Check if scaling strategy affects those SW or FW elements who use MA-filter output
   * The strategies, i.e., UNITY_GAIN is prepared from firmware perspective.
   * Check if UNITY_GAIN works also if INPUT_MUX = PS, and if MAX_RESOLUTION
     translates to filter output overflow?
- Input source selection processing system (PS) vs programmable logic (PL)
  * Test input signal dynamic range
  * Test if writing to input filter has any effect when MUX_IN = PL
- Test dynamic range of DSP48 multiplier input from PS.
- Test freeze accumulator
  * See filter output on WView and DAC
  * Add an enumeration for FREEZ and RUN states?
- Filter input register
  * Not readable when INPUT_SOURCE_CONFIG is set to External input. Verify this.
- LP-Filter Large time-constants
  * Neccessary to add extra bits on HW layer or accuracy will be compromized.
    Find limits for the minimum and maximum possible time constants in usec!
- DSP48_Accumulator Max/Min values
  * Max = 2^36 or 2^35 (concidering SHIFT_LEFT = 1 before slicing)
  * Min = 2^17 or 2^16 (concidering SHIFT_LEFT = 1 before slicing)
  * Test both using software and firmware input signals.
- Filter performance (resolution and overflow properties)
  * Vary analog input signal amplitude and frequency
  * Use ADC offset how signal input amplitude and frequency(use DAC+oscilloscope)
- Loading values from SW into accumulator using FILTER_OUT_REG register
  * How it depends on update flags and load init in FILTER_CONFIG_REG register,
    does it overwrite load-in values from FW
  * How does input selector MUX_IN affect this
  * Read filter output to see when imposing data to accumulator
************************************************************************************/


#pragma once
#include <types.hpp>
#include <cmath>
#include <xparameters.h>
#include "xil_io.h"
#include <xspips.h>
#include "basis_io_picozed7030_based_processor_board.h"
#include "HWpar_2L_3Ph_2L_3Ph.h"
#include <filter_block_registers.h>


// As taken from Kjell code. Needed by other IP Cores ??
#define CHANNELS 			4  // Not used ... Forced to 5
#define SHIFTS				(25 + 0 - 5)
#define MULTIPLIER_MAX			((1<<(25-1))-1)
#define SAMPLE_RATE				AD_RAW_SAMPLE_RATE
#define TIME_CONSTANT_SCALE_FACTOR	1000000// [us/s]
#define INPUT_LSHIFT  XPAR_CONVERTER_1_AD_FILTER_BLOCK_INPUT_FRACTIONAL_BITS
#define DSP48_LSHIFT  (XPAR_CONVERTER_1_AD_FILTER_BLOCK_ACCUMULATOR_ADDITIONAL_BITS + \
			XPAR_CONVERTER_1_AD_FILTER_BLOCK_MULTIPLIER_WIDTH - 1) + 1 \
			-( XPAR_CONVERTER_1_AD_FILTER_BLOCK_ACCUMULATOR_ADDITIONAL_BITS + \
			XPAR_CONVERTER_1_AD_FILTER_BLOCK_MULTIPLIER_WIDTH - 1)

class Filter {
/************************************************************************************
                                PUBLIC INTERFACE
************************************************************************************/
public:
	enum e_GAIN_MODE {UNITY_GAIN = 0, MID_RESOLUTION = 1, MAX_RESOLUTION = 2};
	enum e_MuxIn {PROGRAMMABLE_LOGIC = 0, PROCESSING_SYSTEM = 1};
	enum e_MODE {MOVING_AVERAGE = 0, LOW_PASS = 1};

	fp32_t INPUT_2_OUTPUT_RATIO = 1.00;
/************************************************************************************
Initialize m_base_address to base address of selected converter number.
This function must be run during firmware initialization and before calling
any other functions of this class.

@param: id - Converter identification number.
@return: void
@note: None.
************************************************************************************/
void getFWaddress(unsigned int converter_ID, e_MODE filter_instant);


/************************************************************************************
InitializeFilter values and shifts etc. of selected converter number.
This function must be run during firmware initialization ajust after GetFWaddress

@param: id - Converter identification number.
@return: void
@note: None.
************************************************************************************/
const fp32_t initialize(unsigned int converter_ID, e_MODE filter_instant, const fp32_t& T_samp);


/************************************************************************************
Reads output register from filter block (accumulator).
Also possible forcing filter output using function setOutputSamples(...)

@param:	None.
@return: void
@note: None.
************************************************************************************/
const int32_t* getOutSamples(void); // OK

/************************************************************************************
Sets the configuration register for filter IP-core.

bit 0: 1: Inhibit accumulator updates
bit 1  1: Load init values into accumulator.
bit 2  Signal source: 0: Eksternal input_signals 1: Register input_signals.

@param:	index - The bit to twiddle.
@param: value - Boolean set/reset value.
@return: void
@note: None.
************************************************************************************/
void setConfig(uint32_t index, bool value);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
const uint32_t getConfig(void);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void stopFilterUpdate(bool status);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void startLoadValueAccumulator(bool status);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void selectInput(Filter::e_MuxIn selector);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void setInputSample(int channel, const int32_t value);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
const int32_t* getInputSamples(void);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
const uint32_t* getShiftRegisters(void);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
const uint32_t* getMultiplyRegisters(void);

/************************************************************************************
Writes the filter output registers, manually overriding the filter output by force.

@param:	channel - Selects which output channel to overwrite.
@param: value - Specify what value to write into output.
@return: void
@note: None.
************************************************************************************/
void setOutputSample(int channel, const int32_t value); // OK



/************************************************************************************
#HEADER#

Description
 
 - Tuning the filter timeconstant must be given in micro seconds.

@param: void
@return: void
@note: None.
************************************************************************************/
void tuneLPfilterCutFreq(const int32_t timeConstant_us);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void setMultiplyRegister(int channel, uint32_t DSP48_multiply);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
void setShiftRegister(int channel, const uint32_t numb_of_shifts);

/************************************************************************************
#HEADER#

Description

@param: void
@return: void
@note: None.
************************************************************************************/
const fp32_t tuneMAfilterScaleMode(const e_GAIN_MODE mode, const fp32_t& T_samp);

private:
	int32_t m_base_address;
	int m_CH_COUNT = 8;

	int32_t m_input[8];
	int32_t m_output[8];
	uint32_t m_multiplier[8];
	uint32_t m_shifts[8];

	int32_t m_config;
	e_MODE m_mode;
	e_GAIN_MODE e_scaling_strategy;

	int32_t m_multiplier_max;
	int32_t m_time_constant;
	int m_shift;

	uint32_t DSP48_gain = 0;
	uint32_t samples_per_IRQ = 0;

	uint32_t minimum_DSP48_gain = 5;
	uint32_t maximum_DSP48_gain = 5;
	uint32_t average_DSP48_gain = 5;

};


