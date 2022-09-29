
/************************************************************************************

::File Name: Measurements.hpp
::Description: .


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
§ -------- HARDWARE --------- § - FPGA/FIRMWARE - § ------ CPU/SOFTWARE ------ §



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
None

::MINIMUM SOFTWARE SETUP::


::MINIMUM FIRMWARE SETUP::


::NORMAL SOFTWARE USAGE::

*************************************************************************************
::COMPUTE SCALING FACTORS::
LEM_LA205S_SCALING_FW2SW. Multiply software value with these to get actual SI-current.
These are expected scaling values, to be confirmed tested on Picozed and lab setup.
S1S2
0  0 25mA   ::  0.75/2048 * 1/2 * 1/5*1/30 * 2000 * 2 = 0.0048828;
0  1 50mA   ::  0.75/2048 * 1/2 * 1/5*2/30 * 2000 * 2 = 0.0097656;
1  0 125mA  ::  0.75/2048 * 1/2 * 1/5*1/6 * 2000 * 2  = 0.0244141;
1  1 250mA  ::  0.75/2048 * 1/2 * 1/5*2/6 * 2000 * 2  = 0.0488281;

S1 = closest to analog input termination
S2 = farest away from analog input. Strapping S2 there are 2 pins, thus two options
     the correct strapping is connecting the two pins closest to S1.

I_p = I_sw * LEM_LA205S_SCALING_FW2SW
    = 272 * 0.3662109 = 3,32 A

25mA   :: 0.750/2000 * 2/30*800/0.025 = 0.40
V_dc = V_sw * LEM_LV25_800_SCALING_FW2SW
     = 811 * 0.40 = 324 V

INLCUDE INTERNAL CONSTANTS THAT COMPUTE MAX LEM CURRENT BEFORE SATURATION

*************************************************************************************
::ABOUT THIS SOFTWARE MODULE::
m_Iabc + m_Udc comes directly unscaled from ADC_LP-filter block and are collected using
latched RAM IP-Core sampling synchronized with each interrupt pulse (from PWM).

the m_Ia_MA are similarly synchronous sampled but comes from moving-average filter. Thus
additional scaling is needed to account for shifting/scaling within MA-filter.

The goal is for all signals to be scaled into correct SI-units.

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

::RECOMENDED CALIBRATION PROCEDURE::
1 : For the current measurements, connect all the current transducers into a
    series DC circuit, in the circuit also connect a 3-digit DMM. Use a variable
    DC supply with CV and CC to avoid very large currents and able to check the
    calibration over the full sensor range.

2 : With the series circuit disconnected, tune the ADC FW offset to remove the
    most of the inherent bias. It is good idea to adjust the LP-filter so to
    cut off most of the noise as this will make sensor data more stable and
    easier to calibrate offset and scaling.

3 : Energize the series-circuit and check that ampere reading on DMM and the
    current sensors are equal down to the first decimal. This verifies that
    you have set up correct scaling factors according to LEM sensor and
    front-end impedance.

4 : Use the fine-scaling to to get the LEM sensor data equal to DMM down to
    the third decimal. Usually around unity if step 3 is ok. It is important
    to have a sufficient high load current (close to nominal rating) of LEM
    sensor. Due to non-linearities the scaling factor at low loading will
    not be the same as around nominal loading.

5 : For voltage sensor the same procedure applies.
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
1.00 roy.nilsen@ntnu.no          01.12.2019 Original code release.
2.00 thomas.haugan@ntnu.no       10.01.2020 Determined correct formula for scaling
                                            measured voltage/current signals to SI
                                            units.
2.10 thomas.haugan@ntnu.no       13.01.2020 Added comments/documentation.

*************************************************************************************
::IP-CORE MEMORY MAP::

BASE1: Set automatically by Vivado Design Suite during firmware design phase.
BASE2: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation dependent while register offsets are static.

Offset  Register Name                             Access Register Content (32-bit)
------  ---------------------------------------   ------ -------------------------
SYNCHRONOUS SAMPLING LOWPASS FILTER OUT                  UNSCALED SIGNALS
0x0000: REGISTER_ARRAY(BASE_1, 0)                  R     Phase A current
0x0004: REGISTER_ARRAY(BASE_1, 1)                  R     Phase B current
0x0008: REGISTER_ARRAY(BASE_1, 2)                  R     Phase C current
0x000C: REGISTER_ARRAY(BASE_1, 3)                  R     DC-Link voltage

SYNCHRONOUS SAMPLING MOVING AVERAGE FILTER OUT
0x0010: REGISTER_ARRAY(BASE_2, 0)                  R     Phase A current
0x0014: REGISTER_ARRAY(BASE_2, 1)                  R     Phase B current
0x0018: REGISTER_ARRAY(BASE_2, 2)                  R     Phase C current
0x001C: REGISTER_ARRAY(BASE_2, 3)                  R     DC-Link voltage
0x0020: REGISTER_ARRAY(BASE_2, 4)                  R     Number of samples counter
*************************************************************************************
::CONVERSION FACTORS::

LEM_LA205S_SCALING_FW2SW
LEM_LA205S_SCALING_SW2FW
LEM_LV25_800_SCALING_FW2SW
LEM_LV25_800_SCALING_SW2FW
 
ADC_INOUT_RATIO  // Full-scale input 750 mV = 2048 int32_t output

ADC_FRONT_END_BUFFER_GAIN = 2; // AD8131 internal resistor network gain

LEM_LA205S_SENSOR_CONVERSION_RATIO = 2000; // I_p / I_s

LEM_AMP_SENSOR_NO_OF_TURNS = 5;

LEM_LV25800_SENSOR_CONVERSION_RATIO = 800/0.025; // V_p / I_s

Input current signal is converted into voltage signal via the resulting
frontend input impedance. To cover a broad range input signals at best possible
resolution two jumpers can be used to select 2X voltage divider and input impedance
selector (30 ohm = open, 6 ohm = closed). And voltage division 1/2 or 1/1 closed/open
 
ADC_IN_IMPEDANCE_25mA = 30; // Ohm
ADC_IN_IMPEDANCE_50mA = 30/2;
ADC_IN_IMPEDANCE_125mA = 6;
ADC_IN_IMPEDANCE_250mA = 6/2;

*************************************************************************************
::CONSIDERING LEM SENSOR LIMITATIONS::
 - number of windings derates before open hall-loop becomes magnetic saturated
       LEM_LA205S I_Pn = 60 A. 5 windings derates to 60/5 = 12 A

 - nice to know the maxium physical primary/secondary quantities before core saturate
 - helps to select optimal ADC frontend impedance to get maximum lem/adc resolution
 - neccessary to assess contributing scaling components
 - limitations of the overall power circuit (nominal and expected short-circuit amp)
  helps to decide tripping settings and other various other set points
 - This class does not account for any external scaling as it considers the per-unit
   system within framework of two's complement 12-bit ADC [-2048, 2048].
 - This class is intended to scale between 13-bit fixed-point representation in FM
   and SI-units 32-bit floating point software.
 
LEM_LA205S I_max = 300 A (current sensor saturates)
300/5 = 60 A (max current using 5 primary windings)
I_s/I_p = 1/2000 > I_s(max) = 60 * 1/2000 = 30 mA secondary current at LEM saturation.

Note the differences when probing number of samples between two IRQs. Extracting
directly from MA-filter_output[4] is different than the synchronous sampled via
'moving_integral_reg' The relationship is 3070 vs 5010. The former is the correct one
according to ratio of ADC_clk and PWM_switch_freq. Using these consistently is very
important so that the gain is set correct.

** MA-filter_output[0..3] must be used with MA-filter_output[4]
** moving_integral_reg[0..3] must be used with moving_integral_reg[4]
** If done correctly the scaled MA-signals will be identical to LP-filter output

:: Synchronous MA-filter-SI-scaled output Formula::
m_MA_FILTER_I2O_RATIO = 2^14 / (2^1 * 3) (read in every IRQ from the MA-filter selectGainStrategy function
m_Ia_rawMA = (input * #samples * 2^1 * DSP48_gain) / (2^14) = MA_filter_sync_sampled_output
Ia_MA_A = m_Ia_rawMA * FW2SW_scaling * INVERSE_#samples * m_MA_FILTER_I2O_RATIO

m_MA_FILTER_I2O_RATIO is the MA-filter input-to-output gain-ratio depending on the MA-filter
selected gain-mode (UNITY, MID-RES or HIGH-RESOLUTION). This way SI-scaling is dynamically
adapted to MA-filter gain mode.
*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
 - Test and verify tha granular scaling factor formulas
 - Connect a series voltage+current sensors and calibrate accurately using DC-supply
   and precision multimeter. Make sure to get amp/volt to nominal sensor range.
 - Use the scaling factors elsewhere in software, e.g., setpoint for tripping, PWM
   and hysteresis controller. Confirm that tripping and amp regulator actually maps
   to physical currents.
 -
*************************************************************************************/

#pragma once

#include <cmath>
#include <cstdint>
#include <types.hpp>
#include "HWpar_2L_3Ph_2L_3Ph.h"
#include <register_array.h>


#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;

#define NO_OF_CHANNELS_CNT 8 // max 8 physical ADC channels

class Measurements {
public:

	fp32_t Ia_LP_A;				// Scaled to SI-Units
	fp32_t Ib_LP_A;
	fp32_t Ic_LP_A;
	fp32_t Udc_LP_V;

	fp32_t Ia_MA_A;
	fp32_t Ib_MA_A;
	fp32_t Ic_MA_A;
	fp32_t Udc_MA_V;
	fp32_t Ua0_MA_V;
	fp32_t Ub0_MA_V;
	fp32_t Uc0_MA_V;

	fp32_t bit_per_Aa;
	fp32_t bit_per_Ab;
	fp32_t bit_per_Ac;
	fp32_t bit_per_V;

	Measurements(void);
	void getFWaddress(unsigned int converter_number);
	void initialize(unsigned int converter_number, const fp32_t m_MA_arg);
	void fine_tune(int channel, const fp32_t& scalingfactor);
	const fp32_t* getScaledSamples(void);
	const fp32_t* setBit_pr_A(const fp32_t& bits_per_amp);

private:
	/*fp32_t ADC_INOUT_RATIO = 0.750 / 2048; // Full-scale input 750 mV = 2048 int32_t output
	fp32_t ADC_FRONT_END_BUFFER_GAIN = 2; // AD8131 internal resistor network gain. Maybe NOT needed?
	fp32_t LEM_AMP_SENSOR_NO_OF_TURNS = 5;
	fp32_t LEM_LA205S_SENSOR_CONVERSION_RATIO = 2000; // I_p / I_s
	fp32_t LEM_LV25800_SENSOR_CONVERSION_RATIO = 800 / 0.025; // V_p / I_s
	fp32_t LEM_LV25600_SENSOR_CONVERSION_RATIO = 600 / 0.025; // V_p / I_s

	fp32_t ADC_IN_IMPEDANCE_25mA  = 30; // Ohm
	fp32_t ADC_IN_IMPEDANCE_50mA  = 30 / 2;
	fp32_t ADC_IN_IMPEDANCE_125mA = 6;
	fp32_t ADC_IN_IMPEDANCE_250mA = 6 / 2;

	fp32_t AMP_SCALING_FW2SW;
	fp32_t AMP_SCALING_SW2FW;

	fp32_t VOLT_DC_SCALING_FW2SW;
	fp32_t VOLT_DC_SCALING_SW2FW;

	fp32_t VOLT_AC_SCALING_FW2SW;
	fp32_t VOLT_AC_SCALING_SW2FW; */

	fp32_t m_Aa_per_bit;
	fp32_t m_Ab_per_bit;
	fp32_t m_Ac_per_bit;
	fp32_t m_bit_per_Aa;
	fp32_t m_bit_per_Ab;
	fp32_t m_bit_per_Ac;
	fp32_t m_V_per_bit;
	fp32_t m_bit_per_V;

	uint32_t m_Synch_address;
	uint32_t m_MA_address;

	int32_t m_Ia_rawLP;				// Raw LP-filter sync samples
	int32_t m_Ib_rawLP;
	int32_t m_Ic_rawLP;
	int32_t m_Udc_rawLP;
	
	int32_t m_Ia_rawMA;				// Raw MA-filter sync samples
	int32_t m_Ib_rawMA;
	int32_t m_Ic_rawMA;
	int32_t m_Udc_rawMA;
	uint32_t m_N_Ta_on;
	uint32_t m_N_Tb_on;
	uint32_t m_N_Tc_on;

	uint32_t m_number_of_samples;
	fp32_t m_inverse_number_of_samples;

	fp32_t m_d_au;
	fp32_t m_d_bu;
	fp32_t m_d_cu;

	fp32_t m_MA_FILTER_I2O_RATIO;

	fp32_t m_scaled_signals[NO_OF_CHANNELS_CNT];
	fp32_t m_fw2sw_scaling[NO_OF_CHANNELS_CNT];
	fp32_t m_fine_tuning_gain[NO_OF_CHANNELS_CNT]; //need to finetune every channel individually

}; // EOF


