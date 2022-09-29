
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
	1. Definition of Drive parameters. This is parameters to be read from Flash.
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

struct DriveParameters {
	struct MotorStruct {
		e_DRIVE_ID MACHINE_ID = NoN_INITIALIZED;
		fp32_t UaN_V;
		fp32_t IaN_A;
		fp32_t UfN_V;	  // UfN=Rf*IfN=114*1.6=182.4
		fp32_t IfN_A;
		fp32_t Nn_rpm;
		fp32_t iamax_pu;
		fp32_t ifmax_pu;
		fp32_t Ra_Ohm;    // Ohm
		fp32_t La_H;      // Henry
		fp32_t Tf;        // [s]
		fp32_t Uabasis_V;
		uint32_t polepairs=1;
		uint16_t Machine_Type;
	} MotorData;

	struct DriveStruct {
		fp32_t 		Udc_conv1_StartLim_V;
		fp32_t 		Udc_conv2_StartLim_V;

		// Limiter function parameters: All parameters in pu (per unit)
		fp32_t 		Speed_max;			// Speed or frequency torque limiter
		fp32_t 		Speed_min;
		fp32_t 		dT_dSpeed;

		fp32_t 		Udc_max_V;			// DC-link torque limiter
		fp32_t 		Udc_min_V;
		fp32_t 		dT_dUdc;
		fp32_t 		Te_maxUdc;
		fp32_t 		Te_minUdc;

		fp32_t 		P_max;				// Power torque limiter
		fp32_t 		P_min;
		fp32_t 		Te_max;
		fp32_t 		Te_min;

		fp32_t 		Tlim_slope_filter;    // time to increase to 1 pu torque [s]
		fp32_t      Time_SpeedFilter;

		fp32_t 		Factor_Timedelay_armature;  // Factor used in tuning of gain in current controllers
		fp32_t      Factor_Timedelay_field;

	} DriveData;
};
