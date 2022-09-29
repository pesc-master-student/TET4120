
/************************************************************************************

::File Name: HysteresisAmpRegulator.hpp
::Description: Software driver for hysteresis-based current regulator.


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
 § ----------  FPGA/FIRMWARE ---------------- § -------- CPU/SOFTWARE -------- §
                  ______________________               ___________________
                 |  HYSTERESIS CURRENT  |             |    HYSTERESIS     |
 GATE_SIGNAL <== |  REGULATOR IP-CORE   |<==AXI-BUS== |  SOFTWARE DRIVER  |
                 |______________________|             |___________________|
                          /\                                   ||
                      ____||____                        _______\/_________
                     |  ERROR   |                      |   REGISTER_ARRAY |
 MEASURED CURRENT ==>| CURRENT  |<=== AMP REFERENCE == |     IP-CORE      |
                     |__________|                      |__________________|

::HARDWARE DEPENDENCIES::
- None

::FIRMWARE DEPENDENCIES::
- SINTEF Energy IP-Core 'comparator_limiter_block_v1_0'
- SINTEF Energy IP-Core 'register_array_v1_00_a'
- SINTEF Energy IP-Core 'signal_operasjon_blokk_v1_00_a'
 

		   IP-Core Configuration Property			 	Recommended Setting
		  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		   Width in					      				14
		   Width out									13
		   Number of channels							3
		   Number of comparators						3
		   Reference selector							0
		   Flipflop Default value						"0"
		   Comparator function Greater than				"0x00000002"
 	   	   Comparator function Less than				"0x00000001"
 	   	   Comparator function Equal					"0x00000002"
 	   	   Register Set Mask							"0x00000002"
 	   	   Register Clear Mask							"0x00000001"
 	   	   Readable clipped signals						0
 	   	   Readable	references							1
 	   	   Separate References							0
     	 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯

Write about how the comparator and set/clear masks interpret into a hysteresis controller.

::SOFTWARE DEPENDENCIES::


::MINIMUM SOFTWARE SETUP::


::MINIMUM FIRMWARE SETUP::


::NORMAL SOFTWARE USAGE::
This driver makes the part of a closed-loop control system.

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 thomas.haugan@ntnu.no  	 02.12.2019 Initial code release with stubs.

*************************************************************************************
IP-CORE MEMORY MAP:

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation depedent while register offsets are static.

Offset	Register Name							  Access Register Content (32-bit)
------	---------------------------------------   ------ ----------------------


*************************************************************************************
NOTIFICATONS:   None.
*************************************************************************************
TODO:
 - What is valid dynamic range for reference registers
 - Later adapt for dynamic hysteresis band adaption to keep fixed frequency. Need to
   incorporate switching frequency counter. Need somewhere to set wanted switching freq,
   then select mode (free-running or fixed adaptive H-Band)

 - Adapt the get xparameters.h constants based on converter 0 or 1 circuit
 - If state machine of current controller is needed, make sure no dead states
 - Consider various current clamping/limiting functions (selectable). Could be:
   1) saturate 2) free-running 3) shut-down
 - Skriv om korrekt vikling og strømretning på LEM-sensor og at amp_error = amp_ref - amp_LEM
 - Viktig detalj for å generere feilstrøm (amp_ref - amp_LEM) så må signal_operasjon inngang plus_minus[1:0]
 mates med korrekt konstant, slik at hvert enkelt signal enten adderes eller subtraheres.
 - Skriv om hva som skjer hvis strømretningskonvensjon er satt feil (vis WatchViewbilder)
************************************************************************************/

#pragma once

#include <types.hpp>
#include <math.h>
#include <xparameters.h>
#include "basis_io_picozed7030_based_processor_board.h"
#include "comparator_limiter_registers.h"
#include "register_array.h"

class HysteresisAmpRegulator {
public:
	fp32_t upperLim = 0;
	fp32_t lowerLim = 0;
	bool const SEPARATE_HYST_BAND = XPAR_CONVERTER_1_HYSTERESIS_CONTROL_SEPARATE_REFERENCES;
    int const CHANNEL_COUNT = XPAR_CONVERTER_1_HYSTERESIS_CONTROL_NUMBER_OF_CHANNELS;
    int const COMPS_PR_CH = XPAR_CONVERTER_1_HYSTERESIS_CONTROL_NUMBER_OF_COMPARATORS;
    enum mode {FREERUN = 0, CONST_SWFREQ = 1};


    HysteresisAmpRegulator();
	void getFWAddress(unsigned int converter_ID);
	void initialize(const fp32_t set_bit_pr_A);

	void setSIScaling(const fp32_t amp_scaling_FW2SW);
	const uint8_t* getOutput(void);

	void getToleranceBand(void);
	void setToleranceBand(fp32_t hysteresis);

	void setCurrentReference(int channel, fp32_t refSignalAmps);
	const fp32_t* getCurrentReferences(void);

private:
	int m_base_address_comparator;
    int m_base_address_register_array;

    int const m_MAX_SIGNAL = pow(2, XPAR_CONVERTER_1_HYSTERESIS_CONTROL_WIDTH_IN - 1);
    fp32_t m_MAX_SIGNAL_A = 10.00;

    uint8_t m_flip_flop;
    int32_t m_int32_references[3];
    fp32_t m_fp32_references[3];
    int32_t m_hysteresis[2*3];
    uint32_t m_comparators;

	fp32_t tolerance_band = 0;
	fp32_t m_AMP_SCALING_FW2SW;
	fp32_t m_AMP_SCALING_SW2FW;

    enum STATE { STOP = 0, RUN = 1 };

};

