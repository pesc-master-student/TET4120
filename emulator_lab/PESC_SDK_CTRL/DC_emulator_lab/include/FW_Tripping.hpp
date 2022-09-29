
/************************************************************************************

::FILENAME:: FW_Tripping.hpp
::DESCRIPTION:: Software driver for short-circuit and DC-link overvoltage tripping.


                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
                   Power Electronic Systems and Components
              Department of Electric Power Engineering, NTNU 2020


::SIGNAL FLOW/PIPELINE::
§ ----------------- FPGA/FIRMWARE ------------------ § ------- CPU/SOFTWARE ------- §
 _________     _____________     ______________               ___________________
|   ADC   |   |  LP-FILTER  |   |   TRIPPING   |             |  OC/OV TRIPPING   |
| IP-CORE |==>|   IP-CORE   |==>|    IP-CORE   |<==AXI-BUS==>|  SOFTWARE DRIVER  |
|_________|   |_____________|   |______________|             |___________________|
                                       ||
                                 ______\/_______
                                |  GATE DRIVER  |
                                |    IP-CORE    |
                                |_______________|

 ::HARDWARE DEPENDENCIES::
- None

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'comparator_limiter_block_v1_0'

::IP-CORE CONFIGRATION::

           IP-Core Configuration Property                 Recommended Setting
          =====================================================================
           Width in                                       13
           Width out                                      13
           Number of channels                             4
           Number of comparators                          2
           Reference selector                             0
           Flipflop Default value                         "0"
           Comparator function Greater than               0x00000002
           Comparator function Less than                  0x00000001
           Comparator function Equal                      0x00000002
           Register Set Mask                              0x00000003
           Register Clear Mask                            0x00000000
           Readable clipped signals                       0
           Readable references                            1
           Separate References                            1
          =====================================================================


* Input channel word length is 13 bits, and is required to match output channel of
  the in-front low-pass filter IP-core.
* Supports four channels generating 4 separate first-order low-pass filters.
* The system requires 2 comparators per channel, implementing equal or greater than
  (EQ/GT) and less than (LT) comparison for upper/lower limit respectively. Usually
  the lower limit is less than zero and symmetric to the upper limit (cur-rents).
* The reference selector is set equal 0, meaning that tripping values can only be
  parameterized from the processing system.
* Comparison function Greater Than is set to 0x00000002, so that comparator 2 becomes
  a GT comparison and asserts high if input signal is greater than trip lim-it.
* Comparator function Less Than is set to 0x00000001 assigning comparator 1 as a LT
  comparison, output of comparator goes high when signal is less the trip limit.
* Comparator function Equal is set to 0x00000002 effectively combining comparator 1
  into a equal or greater than comparison.
* Register Set Mask is set equal 0x00000003 and decides which comparators will
  activate the trip indicator, which must be asserted if the input signal goes beyond
  lower or upper limit. Both comparators (LT and EQ/GT) must be linked to flip-flop.
  Comparator 1 and 2 are represented by bit 0 and 1, hence config 0b11 or 0x3.
* Register Clear Mask is set equal 0x00000000, i.e. no comparators are able to reset
  the fault indication register. Once a flip-flop is set, the only way to clear the
  flip-flop is to nullify it from the processing system.
* Separate references is set equal 1 (true), meaning that each channel can be
  Parameterized with its own tripping setpoints.
* Readable clipped signals is set equal 0 (false) as this feature is not needed.

::SOFTWARE DEPENDENCIES::
- Register definitions in 'comparator_limiter_registers.h'

::MINIMUM SOFTWARE SETUP::
- getFWaddress(...);
- initialize(...);
- clearFaults();

::MINIMUM FIRMWARE SETUP::
- SINTEF Energy IP-Core 'comparator_limiter_block_v1_0'
- SINTEF Energy IP-Core 'ad_converter_serial_receiver_v1_00_a'
- SINTEF Energy IP-Core 'filter_block_v1_0' (recommended)

*************************************************************************************
::FUNCTIONAL DESCRIPTION::

The tripping module accepts an input signal that is compared with a predefined
threshold value. Depending on the trip condition the comparator asserts an indicator
flag, which can be used for monitoring purposes, or hardwired in firmware acting as
a blocking signal. Input signal comes from the ADC IP-core and is pre-filtered via
low-pass filter IP-Core. One or several comparators can be associated with each input
channel. Each comparator is configurable as equal (EQ), less than (LT) and greater
than (GT) comparison between input signal and a software-defined trip limit.

Output of comparators are routed to the output of the IP-core via SET and CLEAR masks.
Comparator status signal is bitwise ANDed with both masks, creating different
tripping schemes. Implemented as logic circuitry in firmware this tripping module
features very low latency (nanonsecond range), being adequate for overcurrent (OC) and
overvoltage (OV) detection. The tripping IP-core is part of converter inner control
loop ensuring converter operating within safe and nominal limits.


   EQ/GT_TRIPLIM-----[ >= ]-------[SET_MASK & COMP]-----\     _____________
                       |                                 \___|    OC/OV    |
INPUT_SIGNAL-----------*                                  ___|  INDICATOR  |------>
                       |                                 /   |_____________|
   LT_TRIP_LIM-------[ < ]--------[SET_MASK & COMP]-----/           |
                                                           CLEAR_SIGNAL_FROM_CPU


The tripping module is configured to monitor measured converter output currents and
DC-link voltage, and will disable the gate driver IP-core (block gate pulses) if any
input signal goes outside valid range. Comparator 1 checks the signal against lower
operational limit, and comparator 2 checks the signal upper against upper limit.
Any of the 3OC+1OV conditions disable the gatedriver IP-Core, shutting down converter.

SET_MASK = 0x3 means that either both COMP1 or COMP2 can set the trip fault indicator.
CLEAR_MASK = 0x0 means that no comparator is able to reset the output flip-flop, as a
overcurrent or overvoltage condition requires permanent system shutdown. These sticky
bits can only be cleared from the software layer.

This tripping module is fast and accurate enough to be used as the primary protection
of the converter system, replacing converter onboard short-circuit protection logic.
It can also fit into a larger protection scheme of multiple protection relays, where
proper tuning gives selectively between tripping module, physical gatedriver short-
circuit and power circuit automatic circuit breakers.

The firmware component of the tripping module is classified as "definite current"
protection, due to nanosecond latency and very good precision (12-bit resolution).
Selection of tripping setpoints should consider startup conditions, e.g. surge/inrush
currents, and DC-link voltage overshoot that may incidental trip the converter.

Getting this IP-core up and running is really one of the first and most important
tasks along with calibrating and scaling input sensor data!

*************************************************************************************
::MODIFICATION HISTORY::

Ver  Author                      Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 roy.nilsen@ntnu.no          01.12.2019 Original code release.
2.00 thomas.haugan@ntnu.no       14.01.2020 Added functions setCurrentLimit(..),
                                            setDCLinkVoltLimit(..), getComparators(..)
                                            getFlipFlops(..) and getTripLimits(..).
2.10 thomas.haugan@ntnu.no       15.01.2020 Code testing with converter completed.
                                            Verified tripping limits and gate pulse
                                            blocking mechanism in gate driver.
2.20 thomas.haugan@ntnu.no       20.02.2020 Added comments/documentation.
*************************************************************************************
::IP-CORE MEMORY MAP::

BASE_ADDR: Set automatically by Vivado Design Suite during firmware design phase.
Base address and register offsets are implementation dependent.

NO_OF_CH:       Number of channels
COMPS_PER_CH:   Number of comparators per channel
CH:             Channel index
COMP:           Comparator index
ACCESS: R = Read, W = Write, RW = Read & Write, NRW = No Read OR Write

Offset  Register Name                             Access Register Content (32-bit)
------  ---------------------------------------   ------ -------------------------
TRIPPING STATUS INDICATOR FLAGS
0x0000: COMPARATOR_FLIPFLOP_REG(BASE_ADDR)        RW     Bit-Mapping (LSB = 0)
                                                         0: CH0 (Phase A OC)
                                                         1: CH1 (Phase B OC)
                                                         2: CH2 (Phase C OC)
                                                         3: CH3 (DC-link OV)
INDIVIDIUAL COMPARATOR STATUS FLAGS
0x0004: COMPARATOR_FLAG_REG(BASE_ADDR)            R      Bit-Mapping (LSB = 0)
                                                         0: CH0#COMP1 (Phase A LT)
                                                         1: CH1#COMP1 (Phase B LT)

                                                         2: CH2#COMP1 (Phase C LT)
                                                         3: CH3#COMP1 (DC-link LT)

                                                         4: CH0#COMP2 (Phase A EQ/GT)
                                                         5: CH1#COMP2 (Phase B EQ/GT)

                                                         6: CH2#COMP2 (Phase C EQ/GT)
                                                         7: CH3#COMP2 (DC-link EQ/GT)
CLIPPED INPUT SIGNALS
0x0008: COMPARATOR_LIMITER_OUT_REG(BASE_ADDR, 0)   NRW   CH0 Phase A current
0x000C: COMPARATOR_LIMITER_OUT_REG(BASE_ADDR, 1)   NRW   CH1 Phase B current
0x0010: COMPARATOR_LIMITER_OUT_REG(BASE_ADDR, 2)   NRW   CH2 Phase C current
0x0014: COMPARATOR_LIMITER_OUT_REG(BASE_ADDR, 3)   NRW   CH3 DC-link voltage

COMPARATOR TRIPPING SETPOINT VALUES
Z = COMPARATOR_LIMITER_REFERENCE_REG(BASE_ADDR, NO_OF_CH, COMPS_PER_CH, CH, COMP)
0x0018: Z(BASE_ADDR, 4, 2, 0, 0)                   RW    Phase A (LT threshold)
0x001C: Z(BASE_ADDR, 4, 2, 0, 1)                   RW    Phase A (EQ/GT threshold)
0x0020: Z(BASE_ADDR, 4, 2, 1, 0)                   RW    Phase B (LT threshold)
0x0024: Z(BASE_ADDR, 4, 2, 1, 1)                   RW    Phase B (EQ/GT threshold)
0x0028: Z(BASE_ADDR, 4, 2, 2, 0)                   RW    Phase C (LT threshold)
0x002C: Z(BASE_ADDR, 4, 2, 2, 1)                   RW    Phase C (EQ/GT threshold)
0x0030: Z(BASE_ADDR, 4, 2, 3, 0)                   RW    DC-link (LT threshold)
0x0034: Z(BASE_ADDR, 4, 2, 3, 1)                   RW    DC-link (EQ/GT threshold)

*************************************************************************************
::CONVERSION FACTORS::

This software driver parameterizes the IP-core by setting comparator reference value
of each comparator. Threshold values are given as decimal ampere/volt SI-units, which
must be correctly scaled and casted to fixed-point data type before written to
IP-core register. Scaling factors applies for all IP-cores and related SW drivers,
so these must be globally available for all components and software layers.

CURRENT_SCALING_SW2FW = Scaling factor for current channels.
VOLTAGE_SCALING_SW2FW = Scaling factor for DC-link voltage channel.

Scaling factors are set during software driver initialization, and are generated by
the "Measurements.hpp/.cpp" class.

*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
 - During initalization, exercise the tripping module-function, if the flip-flop,
   comparators setpoint registers are all RW-capable. Use in-front signal source
   (output of LP-filter) to fake a very high signal that will cause OC/OV trip.
   and return either BAD_EXIT/SUCSESS and block converter as safety net because
   this is a dangerous unaccepted situation. Remember to use SI-units
 - Implement various time-characteristics (momentaneous to slower response). Possible
   to have several levels of tripping and pre-warning. This can report to upper
   software layers that operational limits are eminent. Software can write directly
   to FF, while the firmware comparators are last resort and max lim in a selectivety scheme.
 - Sanity check input arguments for all functions, then return BAD_EXIT/SUCSESS
   status in later implementations.
 - Check if resetting the flip-flop from SW will be immediately overwritten by FW if
   the trip condition still applies.
 - Implement an enumeration that quickly indicates which specific fault or faults
   caused the tripping condition. Information available either in register
   COMPARATOR_FLIPFLOP_REG(BASE_ADDR) or COMPARATOR_FLAG_REG(BASE_ADDR).
 - Update void getTripLimits(void) so that it also converts the fetched trip-limits
   into human readable SI-units. Also return a pointer to the table of setpoints.
 - Sanity checking should check that setpoints converted into fixed-point do not
   overflow (2^12) according to max range of ADC.
 - Add constants MAX_AMP and MAX_VOLT that signify the maximum tolerance. Use this for
   sanity checking input arguments. Constants based on power converter system ratings.
 - Verify the mapping of channels to bits in flipflop-register.
 - Proposed auxillary functions:
      - forceTrip(int channel) force-write logic 1 to selected flip-flop and it will
        stay logic high until clearFaults(...)
      - bypass() by writing 2^31 to trip limit register, (will never trip)
*************************************************************************************/


#pragma once

#include <cmath>
#include <cstdint>
#include "HWpar_2L_3Ph_2L_3Ph.h"
#include "comparator_limiter_registers.h"
#include <types.hpp>


#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;

class FW_Tripping {
/************************************************************************************
                                PUBLIC INTERFACE
************************************************************************************/
public:
/************************************************************************************
#Default empty constructor for class FW_Tripping.cpp/FW_Tripping.hpp#

C++ compiler normally auto-generate this constructor during instantiation of
FW_Tripping class. Default constructor is empty by purpose, meaning the software
driver must be initialized and configured manually in application code.

This driver is part of general-purpose PicoZed board functionality. Strictly one class
instance is allowed in the entire code base, and must be part of application-
independent firmware layer.

@param: void
@return: New FW_Tripping object ready for linking SW and FW representation
         of FW_Tripping IP-Core.
@note: None
************************************************************************************/
FW_Tripping(void);

/************************************************************************************
#Fetch FW_Tripping IP-Core firmware base address#

Get base address of the 'comparator_limiter_block_v1_0' IP-Core from hardware
definition generated by Vivado. This function must be run during software
initialization and before calling any other functions of this class.

Grabs additional configuration data from xparameters.h, the number of channels
and number of comparators per channel, necessary when indexing into memory of the
IP-core.

@param: Converter identification selecting correct data in a multi-converter system.
@return: void
@note: None
************************************************************************************/
void getFWaddress(unsigned int converter_ID);

/************************************************************************************
#Initialization function setting scaling factors and IP-Core trip limtis#

After the firmware booting process the IP-core registers hold spurious data so
running this function is critical to ensure correct register content. The overcurrent
(OC) and overvoltage (OV) detection unit is currently parameterized with hard-coded
threshold values for tripping limits. This data will be fetched from a parameter data
base in future implementation.

Scaling factors are dictated by the properties of current/voltage transducers, the
analog-to-digital converter (ADC), and its front-end circuitry. These details are
explained further in "Measurement.hpp". Scaling factors are needed to scale and cast
set points in SI-Ampere/Volt units, into the 13-bit fixed-point number system in
firmware layer. The scaling factors are global properties of the software stack and
must be supplied from the "Measurement.hpp/.cpp" class, later from a parameter
data base. Trip limits can be tuned manually if needed using utility functions
setCurrentLimit(...) and setDCLinkVoltLimit(...).

Assuming an AC converter system operating on sinewave currents, the current upper and
lower trip limits are symmetric around x-axis. The EQ/GT comparator. The DC-link voltage
tripping can be set to just below zero volt to avoid undervoltage tripping when DC-link
capacitors are discharged.

The output flip-flip is cleared AFTER initializing comparator threshold values,
elsewise it will immediately and falsely assert the trip indicator.

@param: bit_pr_A is float-to-fixed point conversion factor for current trip limits.
@param: bit_pr_V is float-to-fixed point conversion factor for DC-voltage trip limits.
@return: void
@note: None
************************************************************************************/
void initialize(unsigned int converter_ID, const fp32_t& bit_pr_Aa, const fp32_t& bit_pr_Ab, const fp32_t& bit_pr_Ac, const fp32_t& bit_pr_V);

/************************************************************************************
#Clear IP-Core tripping indicator register#

The tripping status indicator is a 32-bit register mapping one bit per channel
(see IP-Core memory mapping above), and thus provides maximum 32 channels or fault
flags. Writing 0x0000_0000 to this register from software clears the fault indicators
during IRQ service routine. By design comparators can only SET the status register,
while the CLEAR mask inhibits any comparator/IP-core self-resetting itself.

A fault condition must be correctly dealt with by software stack, as only software
has sufficient information on system state to reason about the triggering event and
respond accordingly.Converter gate pulses remain blocked before running this function.

Status indicators are implemented as sticky bits and will remain in such state until
cleared by software command, processing system reset or PicoZed board power booting.
Comparators are purely combinatorial circuits decoupled from software, and will always
set and reset according to the relationship between monitored input signal and preset
threshold limit. This implies that even attempting to reset fault indicators from
software, the comparators will within milliseconds later SET the fault flag if the
tripping condition still applies. This misleading behavior can be avoided when putting
this clearFaults(...) function in the proper spot for graceful shutdown.

@param: void
@return: void
@note: None
************************************************************************************/
void clearFaults(void);

/************************************************************************************
#Fetch IP-Core tripping limits#

Read-back function that iterates through IP-core registers containing the threshold
values for each single comparator, as a number of comparators can be associated with
each input signal. Use this function to keep software up-to-date by retrieving actual
set points from firmware.

Tripping limits are usually static during the life time of the system, so recommended
running this once during software initialization to confirm IP-core parameterization.
Both OC and OV limits are collected.

@param: None
@return: void
@note: None
************************************************************************************/
void getTripLimits(void);

/************************************************************************************
#Manual adjustment of upper and lower current trip limits for specified channel#

Sanity checks the input arguments to ensure valid channel index avoid writing to
illegal memory location, or indexing the voltage channel. Second argument sanity
check ensures that new threshold is within range (cannot be zero and negative or
greater than a max-limit dictated by the systems current rating).

The provided absolute value is translated into a positive/negative pair which is
scaled and casted to equivalent representation in the 13-bit fixed-point firmware
architecture. Converted threshold values are then written to the respective reference
registers for the upper (EQ/GT) and lower (LT) comparators.

@param: phase - Channel number selector valid range [0-2].
@param: limAmplitude - New trip limit current in absolute value SI-unit Ampere.
@return: void
@note: None
************************************************************************************/
void setCurrentLimit(uint32_t phase, fp32_t limAmplitude);

/************************************************************************************
#Manual adjustment of DC-link voltage upper trip limit#

Sanity checks the input argument to ensure that new threshold is within range, i.e,
cannot be less than the lower trip limit, or greater than max voltage rating of the
DC-link capacitors in the power electronics converter.

The provided absolute value is scaled and casted to equivalent representation in the
13-bit fixed-point firmware architecture. Converted threshold value is then written to
the reference registers for the upper (EQ/GT) comparator.

@param: limAmplitude - New upper trip limit in absolute value SI-unit Volt.
@return: void
@note: None
************************************************************************************/
void setDCLinkVoltLimit(fp32_t limAmplitude);

/************************************************************************************
#Fetch IP-core tripping status flags#

Read-back function retrieving the current tripping status flags from the IP-core. The
tripping status indicator is a 32-bit register mapping one bit per channel, and thus
provides maximum 32 channels or fault flags.

::BIT-MAPPING (LSB = bit 0)::
0: CH0 # COMP1 || COMP2 (Phase A OC - Activated either by LT or EQ/GT)
1: CH1 # COMP1 || COMP2 (Phase B OC - Activated either by LT or EQ/GT)
2: CH2 # COMP1 || COMP2 (Phase C OC - Activated either by LT or EQ/GT)
4: CH4 # COMP1 || COMP2 (DC-link OV - Activated either by LT or EQ/GT)


Status flag = 0 (false) indicates normal operation, while status bit = 0 (true)
indicates that a tripping condition was detected. This function is useful when
analyzing fault scenarios, it quickly highlights if there was either a short-circuit
or DC-link overvoltage event, and which channel/phase caused the tripping. However it
cannot differentiate between triggering on lower or upper threshold.

Content of this register is directly available as an output pin of the IP-core, and
can be used as a driver signal to other components in the firmware layer. In the
current design the status flags are routed and utilized as blocking signals for the
gate driver interface (more details explained in "GateDriver.hpp" class).

@param: None
@return: Immutable copy of the 32-bit register containing tripping status flags.
@note: None
************************************************************************************/
const uint32_t getFlipFlops(void);

/************************************************************************************
#Fetch IP-core comparators status flags#

Read-back function retrieving the momentary status of all comparators for every input
channel of the IP-core. The 32-bit register maps one bit per comparator, thus covering
maximum 32 comparators.

::BIT-MAPPING (LSB = bit 0)::
0: CH0 # COMP1 (Phase A LT)
1: CH1 # COMP1 (Phase B LT)
2: CH2 # COMP1 (Phase C LT)
3: CH3 # COMP1 (DC-link LT)
4: CH0 # COMP2 (Phase A EQ/GT)
5: CH1 # COMP2 (Phase B EQ/GT)
6: CH2 # COMP2 (Phase C EQ/GT)
7: CH3 # COMP2 (DC-link EQ/GT)

These are not sticky status bits as the comparators toggle their output high/low
continuously and according to relationship between input and reference signal of each
comparator. The lack of persistence gives this function limited utility, it is
however very useful during testing and parameterization of the IP-core, checking that
every single comparator is alive and correctly responding to its given threshold.

@param: None
@return: Immutable copy of the 32-bit register containing comparator status flags.
@note: None
************************************************************************************/
const uint32_t getComparators(void);

private:
    uint32_t m_base_address;
    uint32_t m_number_of_channels;                // Here 3 currents and one voltage
    uint32_t m_number_of_comparators;             // Two comparators per channel (only one required for voltage)
    uint32_t m_comparator_register;               // Output of comparators
    uint32_t m_flip_flop_register;                // Output of Flip-flops

    int32_t m_Ia_max;                             // Max current in phase a  > 0
    int32_t m_Ia_min;                             // Min current in phase a  < 0
    int32_t m_Ib_max;                             // Max current in phase b
    int32_t m_Ib_min;                             // Min current in phase b
    int32_t m_Ic_max;                             // Max current in phase c
    int32_t m_Ic_min;                             // Min current in phase c
    int32_t m_Udc_max;                            // Max voltage in DC-link   >0
    int32_t m_Udc_min;                            // Min voltage in DC-link   =< 0

    fp32_t m_Ia_max_A;                             // Max current in phase a  > 0
    fp32_t m_Ia_min_A;                             // Min current in phase a  < 0
    fp32_t m_Ib_max_A;                             // Max current in phase b
    fp32_t m_Ib_min_A;                             // Min current in phase b
    fp32_t m_Ic_max_A;                             // Max current in phase c
    fp32_t m_Ic_min_A;                             // Min current in phase c
    fp32_t m_Udc_max_V;                            // Max voltage in DC-link   >0
    fp32_t m_Udc_min_V;                            // Min voltage in DC-link   =< 0

    fp32_t m_bit_pr_Aa;
    fp32_t m_bit_pr_Ab;
    fp32_t m_bit_pr_Ac;
    fp32_t m_bit_pr_V;

    int32_t m_comp_ref_register[8];

}; // EOF
