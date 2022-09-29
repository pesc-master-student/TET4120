
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
	1. Definition of structs to be used i database for configuration fo the drive
	   Sub-structs:
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

#include "SystemParameters.hpp"
#include "FirmWareParameters.hpp"
#include "ConverterParameters.hpp"
#include "DriveParameters.hpp"
#include "AppParameters.hpp"
#include "LoadParameters.hpp"

struct ConfigParameters {
	SystemParameters      System1;
	FirmWareParameters    FW1;
	ConverterParameters   Converter1;
	ConverterParameters   Converter2;
	DriveParameters       Drive1;
	AppParameters		  App1;
	LoadParameters        Load1;

	fp32_t				  Test_dead_time;
	fp32_t				  bitPerAmp_Arm;
	fp32_t				  bitPerAmp_Field;
	fp32_t				  bitPerVolt_dc;

};

void GetParameters(void);
