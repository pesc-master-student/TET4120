
/************************************************************************************
::FILENAME:: GateDriver.hpp
::DESCRIPTION:: Software driver for gate driver interface IP-Core.


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
# -- HARDWARE -- # --------- FPGA/FIRMWARE --------- # ------ CPU/SOFTWARE ------ #
                     (DISABLE_SIGNALS)
  _____________     ________|__________                        _________________
 | PHY GDRIVER |==>|    GATE_DRIVER    |<=======AXI-BUS======>|   GATE_DRIVER   |
 | INTFC_BOARD |<==| INTERFACE IP-CORE |<--(PWM_SRC #1)       | SOFTWARE DRIVER |
 |_____________|   |___________________|<--(PWM_SRC #2)       |_________________|
                            |                                          |
                    ________|_________                                 |
                   | SWITCHING EVENT  |                                |
                   | COUNTER IP-CORE  |------------AXI-BUS-------------*
                   |__________________|


::HARDWARE DEPENDENCIES::
 - AN 00.12.49 Driver interface board, v4.0 - Documentation
 - AN 01.12.82 1 Siffer Hex Displaykort v2.0 Beskrivelse
 - AN 01.12.17 Gatedriver SMD2
 - AN 01.12.12 20 kVA IGBT omformer. Beskrivelse
 - AN 11.12.10 60 kVA Laboratory converter unit. Documentation
 - AN 09.12.25 Driver interface board for Semikron SEMIKUBE GB or GD drivers V1.01

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'driver_interface_v1_0'
- SINTEF Energy IP-Core 'switching_event_counter_v1_0'

          DRIVER_INTERFACE
          IP-Core Configuration Property                 Recommended Setting
         =====================================================================
          Number Of Signal Sources                       2
          Separate Signals for Lower Drivers             0
          Config Reg Default Value                       0x00131313
          Signal Source Ref Default Value                0x00000000
          Use Watchdog Timer                             1
          Pilot Signal Bitnr                             6
          Number Of disable In Signals                   4
         =====================================================================

          SWITCHING_EVENT_COUNTER
          IP-Core Configuration Property                 Recommended Setting
         =====================================================================
          Number of channels                             6
          Width out                                      16
          Channel nr out                                 0
         =====================================================================

DRIVER_INTERFACE
* Supports two gate signal input sources. The gate signals driving the connected
  converter can be multiplexed from either the pulse-width modulator (PWM) or from
  the hysteresis controller, selecting voltage or current mode operation.
* Separate Signals For Lower Driver equal 0 (false). Each bridge leg uses a common
  gate control signal for both upper and lower gate driver. The converter driver
  interface board uses logic to always ensure that lower gate driver signal is the
  inverse of the upper, avoiding cross-conduction that will short-circuit the
  converter DC-link. Selecting separate gate signals for lower gate drivers requires
  totally 6 signals, while the signals sources only generate pulse signals for the
  upper transistors. Dead time is not available in this IP-core and is assumed taken
  care of by the physical gate driver interface card.
* Default configuration register equals 0x00131313. This register controls what kind
  of gate signal is emitted from the IP-core and routed to the converter gate driver
  interface card. Each bridge leg is represented as a byte in this 32-bit
  configuration register. Right nibble (LSB) configures the lower transistor gate
  driver, and left nibble (MSB) the upper transistor gate driver. Configuration 0x13
  translates to lower transistor gate signal latched HIGH (0x3) and upper transistor
  gate driver is configured 0x1, meaning whatever gate signal source is selected at
  input stage multiplexer.
* Default signal source selection register is set equals 0x00000000, meaning all gate
  driver channels receive gate pulse signal from source 0, and the converter is
  prepared to operate in PWM or voltage control mode.
* Use watchdog timer equals 1 (true) meaning an internal down-counter is implemented
  in the IP-core. This watchdog timer decrements every firmware clock tick and will
  eventually count to zero if not refreshed. The timer must be refreshed every time
  the software IRQ service routine runs. Software stack can go stale due to a number
  of unforeseen reasons, in which the watchdog will expire and disable the gate driver
  signals as a safety precaution.
* Bit 6 of the watchdog timer register is picked out and used as a pilot signal for
  the drivers. As the timer periodically counts down and gets refreshed this creates
  a sawtooth waveform. Extracting a bit of this periodic signal generates a regular
  pulse pilot signal as long as the watchdog is continuously updated. If the pilot
  signal disappears the gate drivers will be blocked due to hardware interlock condition.
* 4 disable signals can block the gate driver pulses during tragic events. These
  disable signals are 1) external hardware interlock, 2) overcurrent (OC) tripping,
  3) DC-link overvoltage (OV) tripping and 4) external watchdog expired.

SWITCHING_EVENT_COUNTER
* Channel count equals 6, measuring the switching frequency of all six transistors
  when assuming a two-level three-phase converter topology.
* One of the 6 pulse counters can be routed to IP-core output pin. In this case
  event counter of channel 0 (Transistor A-) is selected.
* Word size of output pin equals 16-bits, meaning the 16 least significant bits of
  the internal 32-bit counter are sliced out and made available, e.g. for monitoring
  on the digital-to-analog converter (DAC).


::SOFTWARE DEPENDENCIES::
- Register definitions in 'driver_interface_registers.h'
- Register definitions in 'switching_event_counter_registers.h'

::MINIMUM SOFTWARE SETUP::
- getFWaddress(...);
- initialize(...);
- getStatusRegister(...);
- setONOFF(...);
- reload_internal_wtchdog(...);
- setDriverSource(...);

::MINIMUM FIRMWARE SETUP::
- SINTEF Energy IP-Core 'driver_interface_v1_0'
- SINTEF Energy IP-Core 'switching_event_counter_v1_0'

*************************************************************************************
::GATE SIGNAL SELECTION::
- The IP-core offers flexibility to select what gate signal is transmitted to each
  individual gate driver. A 2-stage multiplexer selects the signal.
- Level 1 multiplexer (MUX_L1) is a 4-bit mux allowing up to 16 different sources of
  gate signals, e.g. multiple types of modulators and current regulators.
- MUX_L1 output is split into original and inverted version as input to MUX_L2.
- Level 2 multiplexer (MUX_L2) selects between normal and inverted input gate signal,
  or it can latch the output signal to 1 (LOGIC_HIGH) or 0 (LOGIC_LOW).

                 LOGIC_LOW---------*
         LOGIC_HIGH-------------*  |
                                |  |
(SRC_PWM)-->[MUX_L1]---*------>[MUX_L2]----gatedriver_signal---->(GD_INTERFACE_BOARD)
                       |            |
                       *->[INVERT]--*

- When configuring the IP-core in Vivado, choosing separate gate signal for upper
  and lower transistor per bridge leg is possible. This option will generate an extra
  input pin acting as separate signal source for lower transistor MUX_L1.
- In most cases involving NTNU/SINTEF converter systems, MUX_L2 = 0x13 is the normal
  and recommended operational mode.
- Cross-conduction interlocking logic and blanking time are not implemented by the
  gate driver interface IP-core. These are assumed implemented on physical gate driver
  interface board, closer to the gate driver circuitry.

*************************************************************************************
::GLOBAL ENABLE SIGNAL::
- Gate driver signals distributed from the gate driver interface board are blocked by
  a global enable/disable signal transmitted from the IP-core. Each gate signal is
  blocked when global ENABLE = 0 and active if ENABLE = 1.
- The global enable signal is the result from boolean AND-operation of four pre-
  conditions to be valid for the converter to operate.

  (disable_in_summary = FALSE)----------*
 (watchdog_has_expired = FALSE)-------* |
                                      | |
(enable_signal_reg = TRUE)---------->[AND]---global_enable_signal--->(GD_INTERFACE_BOARD)
                                       |
    (async_reset_sig = FALSE)----------*


- P1 (disable_in_summary = FALSE)
     If any disable flag is asserted this indicates a serious fault condition which
     should turn OFF the power converter system.

- P2 (watchdog_has_expired = FALSE)
     If the internal watchdog counts to zero it will expired, indicating that the
     firmware layer is free-running while the software stack has halted. The power
     converter must shut down, and the entire FW/SW control system requires rebooting.

- P3 (enable_signal_reg = TRUE)
     If the system is in nominal state this signal is used as the ON/OFF command
     provided as software command.

- P4 (async_reset_sig = FALSE)
     Not really a failure flag. This signal is needed to synchronize the behavior of
     the IP-core with AXI-bus asynchronous reset system.

*************************************************************************************
::WATCHDOG::
- Watchdog timers is a simple and effective measure to detect computer malfunctions,
  and is very common in embedded systems.
- The principle is a down-counter initialized with a counter value during software
  booting. The counter is implemented in firmware and ticks along with the global
  firmware clock signal, decoupled from the processing system running the SW stack.
- After successful execution of IRQ service routine the software must refresh the
  watchdog counter value register (kicking the dog), to avoid the counter reaching
  zero, meaning it has expired.
- Expired watchdog indicates failure in either software or the computer system
  running the software.

*************************************************************************************
::DISABLE SIGNALS::

- The IP-core accepts maximum 4 disable signals from surrounding firmware. Their
  purpose is to force the gate driver global_enable_signal LOGIC_LOW, in case of
  serious fail scenarios reported by one of these external disable flags.

      (NOT external_HW_interlock)------------*
         (overcurrent (OC) tripping)-------* |
                                           | |
 (overvoltage (OV) tripping)------------->[AND]---disable_in_summary--->
                                            |
             (ext_watchdog_expire)----------*


- EXT_HW_INTERLOCK
  The SINTEF mezzanine card includes a pilot signal interlock circuit which accepts
  a high-frequency pulse train (approx. 1 MHz) as input. Unstable or absent pilot
  signal will trigger the interlock circuit to disable the gate driver. This pilot
  signal is generated inside the gate driver interface IP-core by slicing out one bit
  from the 32-bit watchdog counter register, forming a closed feedback loop between
  hardware interlock circuit and the IP-core. Trigger condition for this signal is
  either expired watchdog (software failure), or due to invalid configuration of the
  FPGA (firmware failure).

- OVERCURRENT TRIPPING (OC)
  Signal comes from firmware tripping IP-core (documentation in FW_Tripping.hpp/cpp).
  LOGIC_HIGH indicates that overcurrent condition was detected for one of the bridge
  legs of the converter. This is a latched tripping signal that transit back to
  LOGIC_LOW only when measured current is within upper/lower threshold limit, AND
  upon explicit CLEAR command from software stack to OC/OV-tripping firmware module.

- DC-LINK OVERVOLTAGE TRIPPING (OV)
  Signal comes from firmware tripping IP-core (documentation in FW_Tripping.hpp/cpp).
  LOGIC_HIGH indicates that DC-link overvoltage condition was detected. This is a
  latched tripping signal that transit back to LOGIC_LOW only when DC-link voltage
  level comes below upper threshold limit, AND upon explicit CLEAR command from
  software stack to OC/OV-tripping firmware module.

- EXT_WATCHDOG_XPIRE
  Current software/firmware system consists of a twin converter control systems, and
  each converter sub-circuit emits a watchdog_expired signal to the other converter
  firmware controller. When internal watchdog counter expires this disable signal
  ensures mutual and graceful shutdown of both converters.

*************************************************************************************
::SWITCHING FREQUENCY COUNTER::
- The switching event counter IP-core was added to the SINTEF IP-core catalog years
  after the gate driver interface module was first implemented. This software driver
  interfaces both IP-cores as they are naturally connected in the firmware layer.

- Pulse counter is intended for measuring power transistor switching frequency.
  Main components are a rising edge detector and impulse counter. The edge detector
  triggers when the gate signal transitions from LOGIC_LOW to LOGIC_HIGH and emits an
  impulse to the counter. The counter increments at each impulse from edge detector.

- Software driver can regularly sample the switching counter register in IP-core, and
  compare old and new counter value to derive the switching frequency:

     temp1 <= SWITCHING_EVENT_COUNTER_REG
     temp2 <= temp1 - pulse_counter_old;
     pulse_counter_old <= temp1;
     switching_frequency <= temp2 * sampling_rate / counter_update_rate

- Setting counter_update_rate = 1 means that switching frequency will be computed
  every software interrupt. Increasing this variable will decrease the accuracy and
  update rate. Sampling_rate is the known frequency at which the software is executed.
  Correct tuning of these two rates is required to obtain correct switching frequency.
  Update rates greater than 1 means down-sampling, and the measurement is skipped
  between some IRQ cycles.

- The counter is stored in a 32-bit register of range [-2^31 - 1, +2^31]. Counter
  register value will overflow and wrap around, however this does not affect computed
  switching frequency due to the modulo properties of binary addition.

*************************************************************************************
::HEXADECIMAL STATUS ENCODING::

Following tables depict the mapping between binary, decimal and corresponding
hexadecimal encoding.

NTNU CONVERTER SYSTEM
BINARY    DECIMAL   HEXCODE   DESCRIPTION
========= ========= ========= ==============================
0b01111  : 15      : 0x0F      +5VDC SUPPLY FAILURE / STARTUP DELAY
0b01110  : 14      : 0x0E      GATE DRIVER ERROR # PHASE A+
0b01101  : 13      : 0x0d      GATE DRIVER ERROR # PHASE B+
0b01100  : 12      : 0x0C      GATE DRIVER ERROR # PHASE C+
0b01011  : 11      : 0x0b      ERROR GATEDRIVER # BREAKING CHOPPER
0b01010  : 10      : 0x0A      GATE DRIVER ERROR # PHASE A-
0b01001  : 9       : 0x09      GATE DRIVER ERROR # PHASE B-
0b01000  : 8       : 0x08      GATE DRIVER ERROR # PHASE C-
0b00111  : 7       : 0x07      Signal 7 - RESERVED_SIG7
0b00110  : 6       : 0x06      HEATSINK THERMAL LIMIT TRIPPING
0b00101  : 5       : 0x05      Signal 5 - RESERVED_SIG5
0b00100  : 4       : 0x04      Signal 4 - RESERVED_SIG4
0b00011  : 3       : 0x03      DC-LINK MAXIMUM VOLTAGE LIMIT
0b00010  : 2       : 0x02      Signal 2 - RESERVED_SIG2
0b00001  : 1       : 0x01      Signal 1 - MAIN CONTACTOR OPEN
0b10000  : 256     : 0x00      OK - READY FOR OPERATION


SEMIKRON SEMIKUBE CONVERTER SYSTEM
BINARY    DECIMAL   HEXCODE   DESCRIPTION
========= ========= ========= ==============================
0b01111  : 15      : 0x0F      +5VDC SUPPLY FAILURE / STARTUP DELAY
0b01110  : 14      : 0x0E      FAIL SIGNAL GATE DRIVER - PHASE A
0b01101  : 13      : 0x0d      FAIL SIGNAL GATE DRIVER - PHASE B
0b01100  : 12      : 0x0C      FAIL SIGNAL GATE DRIVER - PHASE C
0b01011  : 11      : 0x0b      Signal 11 - RESERVED_SIG11
0b01010  : 10      : 0x0A      OVERCURRENT TRIP (OCT) 450 A # PHASE A
0b01001  : 9       : 0x09      OVERCURRENT TRIP (OCT) 450 A # PHASE B
0b01000  : 8       : 0x08      OVERCURRENT TRIP (OCT) 450 A # PHASE C
0b00111  : 7       : 0x07      EMERGENCY STOP BUTTON ACTIVATED
0b00110  : 6       : 0x06      EXTERNAL INTERLOCK SIGNAL OPEN
0b00101  : 5       : 0x05      CIRCUIT BREAKER TRIP
0b00100  : 4       : 0x04      OVERTEMPERATURE TRIP (OTP) 110 deg/celcius
0b00011  : 3       : 0x03      DC-LINK OVERVOLTAGE VOLTAGE TRIP (OVP) 1000 VDC
0b00010  : 2       : 0x02      Signal 2 - RESERVED_SIG2
0b00001  : 1       : 0x01      MAIN CONTACTOR OPEN
0b10000  : 256     : 0x00      OK - READY FOR OPERATION

* Encoding scheme is somewhat semi-standardized and above are the most common
  encodings, but note that alternative schemes might occur depending on how the gate
  driver interface board is configured.

* Hexadecimal encoding requires 5 status bits to correctly mirror the actual status
  codes visible on the seven-digit display of the driver interface board. Theoretical
  number of unique status codes is 2^5 = 32, however only 16 encodings are considered
  valid. Remaining 16 invalid bit combinations will not appear because of the working
  principle of logic circuitry generating these status bits. In a multi-fauilure
  scenario the monitoring system will report fault code of highest hexadecimal value,
  shadowing faults of lower priority.

* Hexadecimal status encoding is included in the enum e_status_code.

* The driver interface board features a status monitoring system that will block all
  gate driver signals if not all status signals are OK.
  Reading the hexadecimal status word is necessary to keep the firmware and software
  stack in sync and respond correctly to the driver interface board.

* If the converter gate driver interface board jumps to a fault-state it will lock and
  remain in this state until the interface board is power rebooted. Blocking signals
  Signal 1 (0x01 = MAIN CONTACTOR OPEN) and Signal 2 (0x02 = RESERVED_SIG2) do not
  have latching property. A dedicated push button can be used to manually reset the
  SR-latches, putting the converter back to state 0x00 (OK - READY FOR OPERATION).
  The driver interface board features an automatic status reset sequence which is
  activated using a jumper on the board. The Semikron SEMICUBE converter system
  features a dedicated reset circuit that is remotely controllable from software.

*************************************************************************************
IP-CORE MEMORY MAP:

BASE1: Set automatically by Vivado Design Suite during firmware design phase.
BASE2: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base addresses are implementation dependent while register offsets are static.

PHY_GDI = Physical Gate Driver Interface

Offset  Register Name                            Access Register Content (32-bit)
------  ---------------------------------------  ------ ----------------------
GATE DRIVER STATUS REGISTER
0x0000: DRIVER_STATUS_REG(BASE1)                        Bit-Mapping (LSB = 0)
                                                 R      0: PHY_GDI - Status Code T0
                                                 R      1: PHY_GDI - Status Code T1
                                                 R      2: PHY_GDI - Status Code T2
                                                 R      3: PHY_GDI - Status Code T3
                                                      4-7: UNDEFINED
                                                 R      8: PHY_GDI - OK Signal
                                                     9-15: UNDEFINED
                                                 R     16: PHY_GDI - Gate Signal A-
                                                 R     17: PHY_GDI - Gate Signal A+
                                                 R     18: PHY_GDI - Gate Signal B-
                                                 R     19: PHY_GDI - Gate Signal B+
                                                 R     20: PHY_GDI - Gate Signal C-
                                                 R     21: PHY_GDI - Gate Signal C+
                                                    22-23: UNDEFINED
                                                 R     24: PHY_GDI - Resulting Global ENABLE
                                                 R     25: Copy bit 0 of register DRIVER_ENABLE_REG(BASE1)
                                                 R     26: Internal watchdog status. COUNTING = 0, EXPIRED = 1
                                                 R     27: PHY_GDI - Copy Remote RESET command - Reset gate driver interface state logic
                                                 R     28: Disable 1 - HARDWARE INTERLOCK
                                                 R     29: Disable 2 - TRIP OVERCURRENT
                                                 R     30: Disable 3 - TRIP DC-LINK OVERVOLTAGE
                                                 R     31: Disable 4 - EXTERNAL WATCHDOG EXPIRED

COMMAND ENABLE GATEDRIVER
0x0004: DRIVER_ENABLE_REG(BASE1)                RW    Bit-Mapping (LSB = 0)
                                                       0: SW-driven enable/disable signal. ON = 1, OFF = 0
                                                    1-31: UNDEFINED

INPUT MULTIPLEXER LEVEL 2 (MUX-L2)
0x0008: DRIVER_CONFIG_REG(BASE1)                RW    Bit-Mapping (LSB = 0)
                                                      1-0: Driver A-
                                                      3-2: UNDEFINED
   -----------------------                            5-4: Driver A+
    0x0 = Permanent OFF                               7-6: UNDEFINED
    0x1 = PWM Signal                                  9-8: Driver B-
    0x2 = Inverted PWM signal                       11-10: UNDEFINED
    0x3 = Permanent ON                              13-12: Driver B+
   -----------------------                          15-14: UNDEFINED
                                                    17-16: Driver C-
                                                    19-18: UNDEFINED
                                                    21-20: Driver C+
                                                    26-22: UNDEFINED
                                                       27: Remote RESET command - Reset gate driver interface state machine
                                                    31-28: UNDEFINED


INPUT MULTIPLEXER LEVEL 1 (MUX-L1)
0x000C: DRIVER_SIGNAL_SOURCE(BASE1)                   Bit-Mapping (LSB = 0)
                                                      3-0: Driver A-
   ------------------------------------               7-4: Driver A+
    0x0 = SRC00_PWM_VOLT_MODE                       15-12: Driver B+
    0x1 = SRC01_HYSTERESIS_CURRENT_MODE             19-16: Driver B-
    0x2-0xA = UNDEFINED                             23-20: Driver B+
   ------------------------------------             23-20: Driver B+
                                                    31-24: UNDEFINED

WATCHDOG COUNTER
0x0010: DRIVER_WATHCDOG_REG(BASE1)               RW     Watchdog down-counter value

Offset  Register Name                            Access Register Content (32-bit)
------  ---------------------------------------  ------ ------------------------
SWITCHING EVENT COUNTER
0x0000: SWITCHING_EVENT_COUNTER_REG(BASE2, 0)     RW     Pulse counter # Driver A-
0x0004: SWITCHING_EVENT_COUNTER_REG(BASE2, 1)     RW     Pulse counter # Driver A+
0x0008: SWITCHING_EVENT_COUNTER_REG(BASE2, 2)     RW     Pulse counter # Driver B-
0x000C: SWITCHING_EVENT_COUNTER_REG(BASE2, 3)     RW     Pulse counter # Driver B+
0x0010: SWITCHING_EVENT_COUNTER_REG(BASE2, 4)     RW     Pulse counter # Driver C-
0x0014: SWITCHING_EVENT_COUNTER_REG(BASE2, 5)     RW     Pulse counter # Driver C+

*************************************************************************************
::MODIFICATION HISTORY::

Ver  Author                      Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 kjell.ljokelsoy@sintef.no   01.01.2008 Original code release.
2.00 thomas.haugan@ntnu.no       16.12.2019 Added comments, code massage. Renamed files
                                         from Driver_interface to GateDriver.
2.10 thomas.haugan@ntnu.no       12.01.2020 Completed testing and verification in lab
                                         with real power converter.
2.20 thomas.haugan@ntnu.no       25.02.2020 Added documentation on switch freq counter.
2.30 thomas.haugan@ntnu.no       27.02.2020 Documentation completed.

*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
- There are too many getXYZ(...) functions extracting a single bit, cleaner to collect
  everything inside one getStatus(...) function that updates enum words.
- Test if bits [0,1,2,3,8] in DRIVER_STATUS_REG(BASE1) are read/write from software.
- Elaborate on enumerations and their usage.
- Add function setDriverConfigRegister(...) directly controlling MUX_L2.
   * Switching transistor ON/OFF selectively from SW. Purely for test purposes.
   * Check and avoid configurations UPPER = LOWER giving bridge leg cross-conduction.
   * Only configuration 0x13 seems to be working. Closed-loop current control requires
     specific conventions on LEM sensor positive current, and negative feedback-loop
     dictates which transistor in bridge leg switches ON/OFF (current trajectory).
   * Inverse configuration = 0x31 hypothetically should not work at for current mode.
   * Not really many options for correct switching strategy. Upper transistor = MASTER
     and lower transistor = SLAVE. Check both for voltage and current mode operation.
- Investigate the various possible MUX_L1/L2 constellations that support
   * Two-level three-phase converter
   * Two-level single-phase converter
   * It depends on setting separate signals for lower drivers TRUE/FALSE
- SEMiKRON SEMIKUBE setup
    * Test status encodings, enable, reset, etc.
    * Describe various fail-states
    * Necessary to differentiate between standard and SEMIKUBE converter in SW?
- Implement function resetGDIC_FSM(...) sending pulse to 'driver_reset' output pin.
  This resets the driver interface board monitoring system after fault condition.
  DRIVER_CONFIG_REG(BASE1) register. More user-friendly and remote RESET will later
  be available for retrofitted 20 kVA converters.
- During init-function check all status indicators/enums are correctly initialized.
- Function reload_internal_wtchdog(...)
   * Check if reload-value is in range makes difference due to overflow property?
   * Should only allow reloads watchdog timer when gatedriver is OK ?
************************************************************************************/

#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "SystemSWpar.h"
#include "driver_interface_registers.h"
#include "switching_event_counter_registers.h"

#define SAMPLEFREKVENS 4000
#define COUNTER_UPDATE_RATE     4000 //Hz

#define XPAR_CONVERTER_1_SWITCHING_EVENT_COUNTER_WIDTH_OUT 16
#define NO_OF_CHANNELS_CNT XPAR_CONVERTER_1_SWITCHING_EVENT_COUNTER_NUMBER_OF_CHANNELS


class GateDriver {
private:
    uint32_t m_actual_watch_dog_value;
    uint32_t m_base_address_gd;
    uint32_t m_base_address_swfrqcnt;
    const static uint32_t WATCH_DOG_EXPIRE_TIME = 1000; // 2-3x normal update rate. To be calculated form switching frequency . From Parameter Data base FLASH

    uint32_t m_driver_config_reg;
    uint32_t m_status;

    int32_t switching_event_interval_counter;
    int32_t switching_event_counter_update_rate = SAMPLEFREKVENS/COUNTER_UPDATE_RATE;
    int32_t switching_frequency_raw[NO_OF_CHANNELS_CNT];
    int32_t switching_frequency_meas[NO_OF_CHANNELS_CNT];
    int32_t switching_event_count_old[NO_OF_CHANNELS_CNT];

public:
/************************************************************************************
                                PUBLIC INTERFACE
************************************************************************************/
    const static uint32_t m_WATCH_DOG_lOAD_VALUE = (CPU_CLOCK_FREQUENCY/1000000) * WATCH_DOG_EXPIRE_TIME; // reloading the watch dog to avoid time out
    // EXPLAIN THE ENUMS
    enum e_status_code {
        cF_5VDC_FAIL = 15,
        cE_ERR_Ap = 14,
        cD_ERR_Bp = 13,
        cC_ERR_Cp = 12,
        cB_ERR_BR_CHOPPER = 11,
        cA_ERR_An = 10,
        c9_ERR_Bn = 9,
        c8_ERR_Cn = 8,
        c7_RESERVED_SIG7 = 7,
        c6_THERM_LIM = 6,
        c5_RESERVED_SIG5 = 5,
        c4_RESERVED_SIG4 = 4,
        c3_DC_VOLT_TRIP = 3,
        c2_RESERVED_SIG2 = 2,
        c1_CONTACTOR_OPEN = 1,
        c0_OK = 256
    };
    enum e_global_enable {GLOBAL_DISABLE = 0, GLOBAL_ENABLE = 0x1000000};
    enum e_on_off {DISABLE = 0, ENABLE = 1};
    enum e_src {SRC00_PWM_VOLTMODE = 0, SRC01_HYST_AMPMODE = 1, SRC02 = 2, SRC03 = 3,
                SRC04 = 4, SRC05 = 5, SRC06 = 6, SRC07 = 7,
                SRC08 = 8, SRC09 = 9, SRC10 = 10, SRC11 = 11,
                SRC12 = 12, SRC13 = 13, SRC14 = 14, SRC15 = 15,
                INVALID = 0xFF };
    enum e_sw_state {LATCH_LOW = 0, NORMAL = 1, INVERTED = 2, LATCH_HIGH = 3};
    enum e_wtchdog_state {COUNTING = 123, EXPIRED = 388};
    enum e_trip_id {HW_INTERLOCK = 1, TRIP_CURRENT = 2, TRIP_DCLINK = 4, EXT_WTCHDOG_EXPIRED = 8, NORM = 16};

    e_status_code m_hex_status_code;
    e_wtchdog_state internal_watchdog_status = EXPIRED;
    e_trip_id e_disables[4];
    e_src e_pwm_src;
    e_global_enable m_global_enable = GLOBAL_DISABLE;
    e_on_off m_on_off_command = DISABLE;

    const int cnt_signal_sources = XPAR_CONVERTER_1_DRIVER_INTERFACE_NUMBER_OF_SIGNAL_SOURCES;



/************************************************************************************
#Default empty constructor for class GateDriver.cpp/GateDriver.hpp#

C++ compiler normally auto-generates this constructor during instantiation of
FW_Tripping class. Default constructor is empty by purpose, meaning the software
driver must be initialized and configured manually in application code.

@param: void
@return: New GateDriver object ready for linking SW and FW representation
         of GateDriver IP-core.
@note: None
************************************************************************************/
GateDriver(void);

/************************************************************************************
#Fetch GateDriver and EventCounter IP-Cores firmware base addresses#

Get base address of the 'driver_interface_v1_0' and 'switching_event_counter_v1_0'
IP-cores from hardware definition generated by Vivado. This function must be run
during software initialization and before calling any other functions of this class.

@param: Converter identification selecting correct data in a multi-converter system.
@return: void
@note: None
************************************************************************************/
void getFWaddress(unsigned int converter_ID);

/************************************************************************************
#Initialization function configuring gatedriver interface for normal operation#

Most of the essential control register in the gatedriver interface IP-core are
automatically put into correct configuration during firmware booting, according to
the register default values as explained in preamble.

This function mostly repeats that process by letting software configure the registers.
Use this function if the gate driver IP-core needs configuration different from the
default self-parametrized firmware settings. In ordinary circumstances it will be
sufficient to run this configuration process once during system booting, and before
launching any application software.

The global_enable signal is disabled, putting the power converter system in disabled
mode. During startup the converter cannot perform any transistor switching before all
modules in firmware and software are properly initialized.

The watchdog counter register is initialized with a reasonable large counter value to
avoid watchdog expiration.

Gate signal multiplexers are set equal MUX_L1 = 0x0000000 and MUX_L2 = 0x00131313, so
the converter is prepared to operate in PWM voltage mode (PWM_SRC00), and the gate
signal is routed to upper transistor (MASTER) of each bridge leg. Lower transistor is
latched high (SLAVE).

Switching frequency measurement is nullified. Then the counting is started and will
free-run and update according to which control signal is routed through the IP-core.

@param: void
@return: None
@note: None
************************************************************************************/
void initialize(void);


/************************************************************************************
#Fetch state of IP-core gate signal multiplexer MUX_L2#

Read-back function that retrieves current data on driver configuration register. This
register drives the select line for gate signal Level 2 multiplexer. It also contains
the RESET bit used for remotely reseting the gate driver interface board from software.

Details of the driver configuration register and the gate signal multiplexer are
provided in the preamble section.

@param: void
@return: Immutable copy of 32-bit driver interface configuration register.
@note: None
************************************************************************************/
const uint32_t getDriverConfigRegister(void);

/************************************************************************************
#Fetch status register content of gate driver interface IP-core#

Read-back function that retrieves the current data on the status register which is
the most important register of the gate driver IP-core. It contains all information
being transmitted between this gate driver interface firmware module, and the
physical gate driver interface board.

The data includes, gate driver control signals, converter status word and disable
signals due to tripping condition. Detailed explanation available in preamble section.

Function updates the private variable m_status and returns the register content to
caller, which can be used for status diagnosis and drive a finite-state machine.

@param: void
@return: Immutable copy of 32-bit driver interface status register.
@note: None
************************************************************************************/
const uint32_t getStatusRegister(void);

/************************************************************************************
#Fetch status of gatedriver interface disable flags#

Read-back function that grabs current data in the driver status register. The 4 most
significant bits (MSB) of this register contains the four possible disable signals
that signify critical system state requiring system shutdown.

These four disable flags are extracted from the status register using masking. This
function is useful for diagnosing fault conditions.

Details on the status register and related disable signals is found in preamble.

@param: void
@return: Immutable pointer to collection of disable flags.
@note: None
************************************************************************************/
const e_trip_id* get_disable_flags(void);

/************************************************************************************
#Fetch state of IP-core gate signal multiplexer MUX_L1#

Read-back function that retrieves current data on driver signal source register. This
register drives the select line for gate signal Level 1 multiplexer. The purpose of
this mux is to route a selected gate signal source to MUX_L2.

Details of the driver signal source register and the gate signal multiplexer are
provided in the preamble section.

Current implementation of this function reports assumes that all bridge legs are
configured equally. It extracts the signal selector for the lower transistor gate
driver (Driver A-).

@param: void
@return: Immutable copy of MUX_L1 signal selector.
@note: None
************************************************************************************/
e_src getDriverSource(void);

/************************************************************************************
#Select gate signal source - MUX_L1#

Use this function to alter selection lines for the 4-bit MUX_L1, deciding which
signal source that will routed to the converter. Maximum 16 signal sources is supported
so the system can switch between voltage and current control mode, by selecting from
multiple pulse-width modulators or current regulators.

It is first checked that provided argument is valid. If the selection signal is illegal
this will be neglected and the multiplexer remains unchanged. A valid source
identifier is written to the driver signal source register such that all 6
gatedrivers are configured for the same source.

Selecting an input not wired to any gate signal source means that gate driver signals
and transistors remains non-active. Note that this function switches the converter
unconditionally between voltage (SRC_00) and current control mode (SRC_01), and
precautious measures should be provided by software if necessary.

Input multiplexers are decribed in more detail in the preable section.

@param: source - Selects gate signal source, valid range [SRC00 - SRC15].
@return: Immutable copy of driver signal source register.
@note: None
************************************************************************************/
const uint32_t setDriverSource(int source);

/************************************************************************************
#Fetch gate driver interface global_enable flag#

Read-back function that retrieves the global_enable signal. This signal summarizes
the overall state of the gate driver system, and is used for turning ON/OFF the gate
driver signals of the power converter.

The preamble section explains how the global_enable signal is generated. This boolean
flag is extracted from the driver_status_register. GLOBAL_ENABLE means that system
is normal and operation was enabled by explicit command from software. GLOBAL_DISABLE
appears due to one or several blocking conditions (watchdog expiration or OC/OV trip),
or because of explicit disable command from software.

@param: void
@return: Immutable pointer to gate driver interface global_enable signal.
@note: None
************************************************************************************/
const e_global_enable* getGlobalEnable(void);


/************************************************************************************
#Reload internal watchdog counter value#

Use this function to refresh the watchdog counter. The purpose is kicking the dog,
i.e., to notify the firmware layer that software stack is healthy. It is important
where in the running software to place this function. If the intention is to catch
software failure, the end of main-loop is the proper spot.

The watchdog must be refreshed to a counter value large enough that will not expire
before the next IRQ service routine. The down-counting rate of the counter register
is decided by the ratio of firmware layer clock and the computer system clock.


Watchdog counter value affects the LED color and brightness of the pilot signal
interlock circuit: GREEN indicates strong pilot signal, while light intensity decreases
as the counter value comes closer to expiration. LED color alerts watchdog expiration
meaning the watchdog is reloaded with to small value, or the software has indeed
crashed.

@param: value - Counter refresh value. Default value applies if no argument provided.
@return: None
@note: None
************************************************************************************/
void reload_internal_wtchdog(const uint32_t& value = m_WATCH_DOG_lOAD_VALUE);

/************************************************************************************
#Fetch internal watchdog counter value#

Read-back function that retrieves watchdog 32-bit counter register. Use this function
if you need monitor the actual counter value, mostly useful during debugging.

The counter is a 32-bit register of valid range [0, 2^32].

@param: void
@return: Immutable copy of counter register value.
@note: None
************************************************************************************/
const uint32_t get_internal_wtchdog_value(void);

/************************************************************************************
#Fetch status of internal watchdog counter#

Read-back function that retrieves watchdog state from 32-bit driver status register.
This convenience function is useful for checking if watchdog is either EXPIRED or
COUNTING.

The clock cycle the watchdog counter register is checked if equals zero, which raises
a watchdog_expire flag that is available as bit in the status register as well as an
output pin of the gate driver interface IP-core (alerting surrounding firmware).

@param: void
@return: Immutable pointer to watchdog status indicator.
@note: None
************************************************************************************/
const e_wtchdog_state* get_internal_wtchdog_status(void);

/************************************************************************************
#Fetch converter hexadecimal-encoded status code#

Read-back function that retrieves data content on the 32-bit status register. The
hexadecimal status is represented by 4 LSBs (designated T3.T2.T1.T0). The 5th status
bit (Converter OK-signal) is also required to achieve correct decoding.

Relevant bits are extracted when masking the status register by 0x10F, and then casted
into the e_status_code enumeration. Private variable m_hex_status_code is updated and
the function returns a pointer to this variable for external usage.

Details on the hexadecimal encoding is found in preamble section.

@param: void
@return: Immutable pointer to converter status variable encoded in hexadecimal format.
@note: None
************************************************************************************/
const e_status_code* getHWHexStatusCode(void);

/************************************************************************************
#Enable power converter operation - unblock gate signals#

Use this function to command enable gate drivers and acknowledge transistor switching.

@param: void
@return: None
@note: None
************************************************************************************/
void enable(void);

/************************************************************************************
#Disable power converter operation - block gate signals#

Use this function to command disable gate drivers and inhibit transistor switching.

@param: void
@return: None
@note: None
************************************************************************************/
void disable(void);

/************************************************************************************
#Help function updates the driver enable/disable register#

Toggles bit 0 in DRIVER_ENABLE_REG and bit 25 in DRIVER_STATUS_REG. The register
content acts as one of the four pre-conditions that will either enable or disable the
global_enable_signal. Note that bit 25 of DRIVER_STATUS_REG register is read-only
copy so it cannot be used for disable/enable control.

Writing 0 (FALSE) to bit 0 disables and blocks the gate driver signals, while writing
1 (TRUE) will unblock the gate signals. This functions toggles the gate driver signals
ON or OFF. The effect of this function is observable only when all other blocking
conditions (overcurrent, overvoltage or expired watchdogs) are absent.

This function implements the software-controlled ON/OFF signal enabling/disabling
power transistor switching.

@param: flag - ENABLE/DISABLE turns gate signals ON/OFF respectively.
@return: Immutable pointer to actual ON/OFF command signal.
@note: None
************************************************************************************/
const e_on_off* setONOFF(e_on_off flag);

/************************************************************************************
#Fetch frequency counter register value#

Read-back function that retrieves current content of 32-bit pulse counter register.
This function is mostly useful when monitoring the pulse counter during debugging.

The counter is a 32-bit register of valid range [-2^31 - 1, 2^31].

@param: void
@return: None
@note: None
************************************************************************************/
const int32_t* getPulseCounter(void);


/************************************************************************************
#Start switching frequency measurement#

The pulse counter is a free-running counter that ticks away as long as rising edge
transitions are detected in the gate signals. There is an internal interval counter
used to determine how often a new switching frequency should be computed.

This function to initializes the interval counter.

@param: void
@return: None
@note: None
************************************************************************************/
void switching_frequency_measurement_start(void);

/************************************************************************************
#Calculate switching frequency#

The pulse counter is sampled and differential between old and new counter value forms
the basis for computing the switching frequency of each individual power transistor.

Note that the switching event counter is probing the gate signals coming out of the
gate driver interface IP-core, and does not necessarily reflect the true switching
pattern of the converter. Gate signal selector MUX_L2 is normally configured such that
lower transistor gate signal is latched high, while upper transistor receives the
correct gate signal pulse train. It is however safe to assume both transistors
switch at equal rates.

More details on the switching frequency measurement are found in preamble section.

@param: void
@return: None
@note: None
************************************************************************************/
void switching_frequency_measurement(void);


/************************************************************************************
#Remotely reset gate driver finite interface state machine (FSM)#

The gate driver interface board features an automatic reset circuit that puts the
internal monitoring system back to normal state after a fault scenario. Upon critical
faults like OC/OV the converter will latch into fault-state, even after resolving the
critical error the system requires manual fault clearance.

Resetting the gate driver interface board is possible from a bushbutton switch on the
board, alternatively it can be remotely reset by software. Use this function only when
the software has reasonable system information to conclude that clearing fault is safe.

By asserting bit 27 in DRIVER_CONFIG_REG register the reset signal will be transmitted
to the gate driver interface board.

@param: void
@return: None
@note: None
************************************************************************************/
void resetGDIC_FSM();

}; // EOF
