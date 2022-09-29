
/************************************************************************************
::File Name: ParameterConfig.hpp
::Description:


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

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 roy.nilsen@ntnu.no          02.07.2020 Original code release.
2.00  .


*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
-

Key modules
	1. Definition of FirmWare parameter. This is parameters to be read from Flash.
	   In first version they will be hard coded in GetValue-function:
	   	   	 System				   (System information)
	     	 FW                    (FirmWare)
	     	 Converter			   (Two instance of this)
	     	 Drive
	     	 App				   (Application)

	Each of this structs have sub-classes for the different functions, PI, filter,
	MotorData, etc.....

************************************************************************************/

#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "e_DRIVE_ID.hpp"

struct FirmWareParameters {
	struct EmulatorStruct {
		fp32_t Tstep = 1.0e-6;           // Timestep in solver in FPGA
		e_DRIVE_ID MACHINE_ID;             //
		fp32_t Udc1_V;
		fp32_t Udc2_V;
		fp32_t Tm;                       // [s] Mechanical time constant representing the inertia
		fp32_t kn;            			 // Factor in Quadratic load function TL=kn*n^2  [pu]
	} Emulator;
};
