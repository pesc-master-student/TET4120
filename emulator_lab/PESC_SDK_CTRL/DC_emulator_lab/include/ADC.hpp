
/************************************************************************************
::File Name: ADC.hpp
::Description: Software driver for Analog Devices AD9222 analog-to-digital converter.
 
 
                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
            Department of Electric Power Engineering, NTNU 2019
 
 
 
::SIGNAL FLOW/PIPELINE::
# -------- HARDWARE --------- #- FPGA/FIRMWARE - # ------ CPU/SOFTWARE ------ #
 ____________     __________     ___________		  	  ___________________
|   ANALOG   |	 |  AD9222  |   |    ADC    |  	         |  	  ADC	     |
|  FRONTEND  |==>|   CHIP   |==>  IP-CORE   |==AXI-BUS==>|  SOFTWARE DRIVER  |
|____________|	 |__________|   |___________|==PL==>	 |___________________|

::HARDWARE DEPENDENCIES::
- Analog Devices AD9222 (12-bit/40 MSPS, 8 channel, serial LVDS 1.8V)

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'ad_converter_serial_receiver_v1_00_a'

		   IP-Core Configuration Property			 	Recommended Setting
		  ==================================================================
		   AD converter signal width      				12
		   Output signal clock domain selector			"00000000"
		   Init delay counter start value				10
		   Invert output signal							"00000000"
		   Use offset compensation						"1"
		   AD Clock Iobdelay Value						7
 	   	   AD Signal Iobdelay Value						22
 	   	   Use Internal Delayctrl						"1"
 	   	   Downsampling factor width					0
 	   	   Number of channels							8
 	   	   Width out									13
     	 ===================================================================

::SOFTWARE DEPENDENCIES::
- None

::MINIMUM SOFTWARE SETUP::
- ADC::getFWAddress(void) Software points to correct ADC registers for read/write.

::MINIMUM FIRMWARE SETUP::
- ADC::setIObDelay(...) Configures ADC data and clock signals skew correction.
- ADC::setOffset(...)   Calibrates input channels offset.

::NORMAL SOFTWARE USAGE::
- ADC::getSamples(void) Updates SW with latest samples from FW IP-Core.

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 kjell.ljokelsoy@sintef.no   01.01.2008 Original code release.
2.00 thomas.haugan@ntnu.no  	 10.10.2019 Added comments/documentation.Code cleanup.
											and adaption for NTNU PESC FW/SW platform.
 
2.10 thomas.haugan@ntnu.no	     11.10.2019	Refactored from AD.hpp to ADC.hpp.
2.20 thomas.haugan@ntnu.no	 	 06.11.2019 Code improvement/correctness on functions
											resetIObDelay(..), incIObDelay(..),
											decIObDelay(..)and setConfig(..). Added
											constructors and refactored functions name
											according to naming convention.
2.30 thomas.haugan@ntnu.no	 	 12.11.2019 Bug fixing getOffset(..), setOffset(..)
 	 	 	 	 	 	 	 	 	 	 	Added dependency descriptions. Removed
                                            constructors.
 
2.40 thomas.haugan@ntnu.no       12.11.2019 Release-grade code after testing/debug.
2.50 thomas.haugan@ntnu.no       20.11.2019 Clamping mechanism in setOffset(..).
                                            Added comments in ADC dynamic range and
                                            confirmed overflow limits by measurement.
2.50 thomas.haugan@ntnu.no       22.01.2020 Fixed bug in calibrateOffset(..) and added
                                            const pointer as return type to selected
                                            read-back functions.
*************************************************************************************
IP-CORE MEMORY MAP:

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation depedent while register offsets are static.

Offset	Register Name							  Access Register Content (32-bit)
------	---------------------------------------   ------ ----------------------
RAW ADC SAMPLE DATA CORRECTED FOR OFFSET VOLTAGE (AD_S_SIGNAL)
0x0000: AD_S_SIGNAL(BASEADDR, 0)					R     = Channel A + Offset A
0x0004: AD_S_SIGNAL(BASEADDR, 1)					R     = Channel B + Offset B
0x0008: AD_S_SIGNAL(BASEADDR, 2)					R     = Channel C + Offset C
0x000C: AD_S_SIGNAL(BASEADDR, 3)					R     = Channel D + Offset D
0x0010: AD_S_SIGNAL(BASEADDR, 4)					R     = Channel E + Offset E
0x0014: AD_S_SIGNAL(BASEADDR, 5)					R     = Channel F + Offset F
0x0018: AD_S_SIGNAL(BASEADDR, 6)					R     = Channel G + Offset G
0x001c: AD_S_SIGNAL(BASEADDR, 7)					R     = Channel H + Offset H

BIASING INFORMATION UPDATED DURING CALIBRATION (AD_S_OFFSET)
0x0020 	AD_S_OFFSET(BASEADDR, 0)					RW	  Offset A
0x0024 	AD_S_OFFSET(BASEADDR, 1)					RW	  Offset B
0x0028 	AD_S_OFFSET(BASEADDR, 2)					RW	  Offset C
0x002C 	AD_S_OFFSET(BASEADDR, 3)					RW	  Offset D
0x0030 	AD_S_OFFSET(BASEADDR, 4)					RW	  Offset E
0x0034 	AD_S_OFFSET(BASEADDR, 5)					RW	  Offset F
0x0038 	AD_S_OFFSET(BASEADDR, 6)					RW	  Offset G
0x003c 	AD_S_OFFSET(BASEADDR, 7)					RW	  Offset H

IP-CORE CONFIGURATION REGISTER (AD_S_CONFIG)
0x0040	AD_S_CONFIG_BLOCK_NEW_VALUE_FLAG_BITNR		RW	  RWBlocks the signal_out_new flag.
0x0041	AD_S_CONFIG_BLOCK_SIGNAL_UPDATES_BITNR		RW	  Freezes ad register value update.
0x0044	AD_S_CONFIG_STARTUP_BLOCK_IS_ACTIVE_BITNR	RW	  Startup blocking is active. ( If this flag is stuck on the AD clock as failed.)
0x0048	AD_S_CONFIG_IODELAY_RESET_BITNR				RW	  IODELAY reset to default delay.
0x0049	AD_S_CONFIG_IODELAY_UP_BITNR				RW	  IODELAY increments delay one step.
0x0050	AD_S_CONFIG_IODELAY_DOWN_BITNR				RW	  IODELAY decrements delay one step.

*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
- Implement support for serial-peripheral interface (SPI) controller. This will unlock
  many useful services built into the AD9222 chip, especially for automatic IODELAY
  tuning algorithm, generate random noise signals and reading status of the chip.
- Examples available from Xilinx in xspips.
- SPI is routed and independent of ADC IP-Core.

SPI Implementation ideas
		- get offset values for needed registers
		- check Zynq TRM programmers model for SPI controller
		- route SPI bus to GPIO2 pins for debug via oscilloscope
- Test and verify setChannel(...) + clearOverflowIndicators(...)

Comments from Roy Nilsen
- Calibrate functions is probably to simple, implement bias-adjust based on
  several values and run average to find DC component.

************************************************************************************/

#pragma once

#include <types.hpp>
#include <xparameters.h>
#include <stdlib.h>
#include <math.h>
#include <xspips.h>
#include <assert.h>
#include "basis_io_picozed7030_based_processor_board.h"
#include <AD_converter_serial_receiver_registers.h>
#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;

#define IOBDELAY_MAX_TAPS 32

class ADC {
private:
	int m_base_address;
    static const int m_channel_count = XPAR_AD_CONVERTER_NUMBER_OF_CHANNELS;
	int m_iodelay = AD_S_IO_DELAY_INITVALUE;
	int m_channel[m_channel_count];
	int m_offset[m_channel_count];
	bool m_saturation[m_channel_count];
	int m_config;
	int const m_MAX_AMPLITUDE = pow(2, XPAR_AD_CONVERTER_WIDTH_OUT - 1);
	int const m_MIN_AMPLITUDE = -m_MAX_AMPLITUDE;
	int const m_MAX_OFFSET = m_MAX_AMPLITUDE / 2;

/************************************************************************************
#Help function triggers selected edge-sensitive bits in ADC configuration register#

This function creates a positive-edge transition needed specifically to operate the
IOBDELAY primitive inside ADC IP-Core.

BIT SENSE PROPERTY
 8	Edge  Reset IO delay to default value.
 9  Edge  Increment IO delay one notch.
 10 Edge  Decrement IO delay one notch.

@param: index - Bit selection index. Valid index values = [8, 9, 10].
@return: void
@note: None
************************************************************************************/
void risingEdge(uint32_t index); // OK

/************************************************************************************
#Reset ADC IP-Core skew compensation to default value#

Vivado Design Suite has freedom to place and route the IP-Core wherever optimizes
overall firmware design. Mismatching path delay of nets 'ad_d', 'ad_fcon' and
'ad_dcon' creates a skew effect so the IP-Core cannot deserialize data stream
from AD9222 correctly.

@param:	void
@return: void
@note: None
************************************************************************************/
void resetIObDelay(void); // OK

/************************************************************************************
#Help function set/clear selected level-sensitive bits in ADC configuration register#

BIT SENSE PROPERTY
 0	Level Block the 'ad_signal_new_adclk' output port.
 1  Level Freeze output registers.
 4 	Level Startup register output blocking.

@param: index - Bit selection index. Valid index values = [0, 1, 4].
@param: value - Enable/disable bit by writing true/false.
@return: void
@note: None
************************************************************************************/
void setConfigReg(uint32_t index, bool value); // OK

/************************************************************************************
#Help function increments ADC IP-Core skew compensation one delay step#

Adjust IOBDELAY by adding one delay tap (adding delay). Wrap around may occur.

@param: void
@return: void
@note: None
************************************************************************************/
void incIObDelay(void); // OK

/************************************************************************************
#Help function decrements ADC IP-Core skew compensation one delay step#

Adjust IOBDELAY by removing one delay tap (removing delay).Wrap around may occur.

@param: void
@return: void
@note: None
************************************************************************************/
void decIObDelay(void); // OK



/************************************************************************************
								PUBLIC INTERFACE
************************************************************************************/
public:

/************************************************************************************
#Default empty constructor for class ADC.cpp/ADC.hpp#

C++ compiler normally auto-generate this constructor during instantiation of ADC class.
Default constructor is empty by purpose, meaning the software driver must be
initialized and configured manually in application code.

This driver is part of general-purpose Picozed board functionality. Strictly one class
instance is allowed in the entire code base, and must be part of application-
independent firmware layer.

::STATIC TESTS/COMPILER PREDICATES::
Eliminate simple errors due to parameter mismatching between firmware and software.
  + P01 - Software driver assumes 8 channel ADC IP-Core.
  + P02 - Assume 13-bit width for output signals.

@param:	void
@return: New ADC object ready for linking SW and FW representation of ADC IP-Core.
@note: None
************************************************************************************/
ADC(void); // OK

/************************************************************************************
#Fetch ADC IP-Core firmware base address#

Get base address of the 'ad_converter_serial_receiver' IP-Core from hardware
definition generated by Vivado.
This function must be run during software initialization and before calling
any other functions of this class.

@param: void
@return: void
@note: None
************************************************************************************/
void getFWAddress(void); // OK

/************************************************************************************
#Fetch new output samples from ADC IP-Core#

Serial data stream from AD9222 chip is decoded and put into readable output registers.
It is safe to assume synchronous sampling property as software runs orders of
magnitude slower than firmware.

Sampling and timing on firmware layer is parallel and synchronized. However expect
minor phase shift between sampling of each channel on software layer due to latency
in AXI-Bus and CPU pipeline.

Updated samples are compared against >= 2048 and turns on a boolean flag, indicating
that full signal range. Saturation flags are sticky bits and must be reset manually.

::AD9222 ADC OUTPUT SIGNALS FORMAT::
- Analog front-end circuitry + ADC chip tolerate +/- 0.750 V voltage input signal
  and signals beyond [-750, 750] millivolt will saturate.
- AD9222 produces 12-bit output in two's complement binary format.

::ADC IP-CORE DIGITAL DYNAMIC RANGE::
- 13-bit ADC IP-Core maps +/-0.750 V analog input to +/- 2048 integer output.
- Firmware layer dynamic output range is dictated by XPAR_AD_CONVERTER_WIDTH_OUT
     MAX_AMPLITUDE      =   m_MAX_AMPLITUDE
     MIN_AMPLITUDE      =  -m_MAX_AMPLITUDE
 
- By example dynamic range of output samples in 13-bit firmware architecture is
     MAX_AMPLITUDE      =   4096 = 0x00001000
     MIN_AMPLITUDE      =  -4096 = 0xFFFFF000

- Extreme values occur at maximum input signal amplitude combined with maximum
  DC offset.
- ADC IP-Core overflow occurs when AC_amplitude + DC_amplitude >= m_MAX_AMPLITUDE.

@param: void
@return: A pointer to the table of output samples.
@note: Immutable pointer to the table of samples.
************************************************************************************/
const int* getOutSamples(void); // OK

/************************************************************************************
#Manual fine-tuning of ADC IP-Core delay/skew compensation mechanism#

Very noisy or deformed ADC output samples calls for skew correction. This tuning
process is needed strictly once per new firmware design. Skew compensation is
implemented using Xilinx IOBDELAY primitive, similar to a shift-register with finite
number delay taps.
 
@param: delay - Number of delay taps introduced by the IOBDELAY block.
@return: void
@note: None
************************************************************************************/
void setIObDelay(uint32_t delay); // OK

/************************************************************************************
#Auto-calibrate ADC IP-Core offset settings during software initialization#

Certain types of hall-effect voltage and current sensors may have inherent offset in
output signal (magnetic remanence). Analog front-end circuits feature voltage drift.

Running this function during software initialization counteracts for any unwanted DC
component in the ADC output signals. Power circuits must be de-energized during this
calibration procedure. IP-Core adds offset to output signal every sampling interval.

@param: void
@return: Immutable pointer to table of offset calibrations.
@note: None
************************************************************************************/
const int* calibrateOffsets(void); // OK

/************************************************************************************
#Fetch channel-specific offset compensation values from ADC IP-Core#

Read-back function retrieving the current offset value associated to specified ADC
output channel.

@param: channel - Channel number selecting which output channel to read offset.
@return: void
@note: None
************************************************************************************/
void getOffset(int channel); // OK

/************************************************************************************
#Adjust offset for specified ADC IP-Core output channel#

Provides manual fine-tuning offset per channel in case initial calibration failed.
Output stage of ADC IP-Core is padded with additional bit(s) to increase dynamic
range of firmware layer. Offset value must be within extra headroom so to avoid
output signal over/underflow. Thus offset parameter is clamped to +/- m_MAX_OFFSET
as the ADC IP-Core output otherwise overflows when signal goes outside valid range.

@param: channel - Channel number selector valid range [0-7].
@param: offset - New offset value of range [-m_MAX_OFFSET, m_MAX_OFFSET].
@return: void
@note: None
************************************************************************************/
void setOffset(int channel, int offset); // OK

/************************************************************************************
#Fetch ADC IP-Core configuration#

Read-back function retrieving the content of firmware configuration register.

BIT SENSE PROPERTY
 0	Level Blocks the 'ad_signal_new_adclk' output port.
 1  Level Freezes ad register value update.
 4 	Level Startup blocking active.
 8  Edge  Resets IO delay to default value.
 9	Edge  Increments IO delay one notch.
 10 Edge  Decrements IO delay one notch.

@param: void
@return: void
@note: None
************************************************************************************/
void getConfig(void); // OK

/************************************************************************************
#Freeze ADC IP-Core output samples data stream#

ADC IP-Core continues decoding data from AD9222 chip, however new samples are not put
available at output stage.
 
Can freeze the output value: saturation, hold sample by command in cases some
application requires/benefits from this.

@param: state - Output samples are blocked/unblocked if flag == 'True/False'
@return: void
@note: None
************************************************************************************/
void freezeOutputSamples(bool state); // OK

/************************************************************************************
#Enable/Disable ADC IP-Core new sample indicators during firmware startup#

Blocks output ports 'ad_signal_new_adclk' and 'ad_signal_new_busclk' the 10 first
samples, allowing all firmware to initialize before releasing new ADC samples.
 
@param: status - Update flags are blocked/unblocked when status == 'True/False'
@return: void
@note: None
************************************************************************************/
void startupBlockOutput(bool state); // OK

/************************************************************************************
#Enable/Disable ADC IP-Core new sample indicators at run-time#

ADC IP-Core provides two output ports 'ad_signal_new_adclk' and 'ad_signal_new_busclk'
signaling new samples to surrounding firmware. The flags are time-synched to FW/SW
respectively. Blocking the flags does however not stall ADC output samples.

@param: status - Update flags are blocked/unblocked when status == 'True/False'
@return: void
@note: None
************************************************************************************/
void blockNewSampleFlag(bool status); // OK

/************************************************************************************
#Force ADC IP-Core output channel to specified signal value and overrides ADC sample#

ADC IP-Core output registers AD_S_SIGNAL are software read and write capable. Can be
exploited to insert overwrite the register content with a software specified value.

The provided value is soon erased at the next interrupt, as the AD9222 ADC and IP-Core
continuously stream fresh samples into the registers.

@param: channel - Channel number selector valid range [0-7].
@param: signal - New sample value of range [-m_MAX_AMPLITUDE, m_MAX_AMPLITUDE].
@return: void
@note: None
************************************************************************************/
void setChannel(int channel, int signal);

/************************************************************************************
#Reset sticky bits in saturation indicator flags#

Function getOutSamples(..) checks if new samples are within normal range of the AD9222
ADC and analog frontend circuit [-2048, 2048]. A signal outside limits is flagged with
a dedicated saturation flag. Can be useful to detect extreme values from voltage and
current sensors. Asserted indicator flag must be reset calling this function.

@param: None
@return: void
@note: None
************************************************************************************/
void clearOverflowIndicators(void);

/************************************************************************************
#Fetch all offset compensation values from ADC IP-Core#

Read-back function retrieving the current offset value of all 8 ADC output channels.

@param: None
@return: Immutable pointer to array of current offset values.
@note: None
************************************************************************************/
const int* getOffsets(void);

}; // EOF


