
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
            Department of Electric Power Engineering, NTNU 2020



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
The IP-Core load input port is asserted, then the load_in_vector is
loaded into the register, and will overwrite/get priority over
processor-written data. One scenario is converter watcdog expiration
will load the register to zero, effectively disconnecting the contactor.


*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 kjell.ljokelsoy@sintef.no   01.01.2008 Original code release.
2.00 thomas.haugan@ntnu.no  	 10.10.2019 Added comments/documentation.Code cleanup.
											and adaption for NTNU PESC FW/SW platform.

*************************************************************************************
IP-CORE MEMORY MAP:

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation depedent while register offsets are static.

Offset	Register Name							  Access Register Content (32-bit)
------	---------------------------------------   ------ ----------------------


*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
-

************************************************************************************/


#pragma once

#include <types.hpp>
#include <xparameters.h>
#include "basis_io_picozed7030_based_processor_board.h"
#include "register_array.h"


class Relay {
public:
	enum relaystatus {OPEN = 0, CLOSED = 1, UNDEFINED = 100};

	void getFWAddress(void);
	void initialize(void);
	void setState(enum relaystatus breakerStatus);
	const relaystatus getState(void);

	void close(void);
	void open(void);


private:
	uint32_t m_base_address;
	uint32_t m_relay_register;
	relaystatus m_state;
};
