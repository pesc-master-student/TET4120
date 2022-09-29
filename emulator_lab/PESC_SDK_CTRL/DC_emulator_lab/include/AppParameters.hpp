
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
	1. Definition of Application parameters. This is parameters to be read from Flash.
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

#include "types.hpp"

struct AppParameters
{

	struct RampStruct
		{
			fp32_t time_increase;			    // Time to increase from 0 to 100 %.
			fp32_t time_decrease;			// Time to decrease from 100% to 0.
			fp32_t error_max;                   // If difference between actual value and reference is larger than this; no updating
			fp32_t deviation_max;               // If larger than this an integrator is used. If less than this value a lowpass filter
		};
		RampStruct TorqueRamp;
		RampStruct SpeedRamp;
		RampStruct SpeedQuickStopRamp;
		RampStruct PowerRamp;
		RampStruct UdcRamp;

		fp32_t TorqueRefFilterTimeConstant;
		fp32_t SpeedRefFilterTimeConstant;
		fp32_t PowerRefFilterTimeConstant;
		fp32_t UdcRefFilterTimeConstant;

		bool   b_Torqueramp_Allowed;
		bool   b_Torqueramp_Fixed_Selected;
		bool   b_Speedramp_Allowed;
		bool   b_Speedramp_Fixed_Selected;
		bool   b_Powerramp_Allowed;
		bool   b_Powerramp_Fixed_Selected;
		bool   b_Udcramp_Allowed;
		bool   b_Udcramp_Fixed_Selected;

		bool   b_UDCref_V_extern_selected;

		struct ControlStruct
			{
				fp32_t Kp;			                // Gain in pu for speed controller
				fp32_t Ti;			                // Integral time constant [s]
				fp32_t Ta;			                // Filter time constant (not in use)
				fp32_t Kv;			                // Feedforward gain. Usually 0 or 1 in our applications

			};
		ControlStruct SpeedController;
		ControlStruct UdcController;

		uint16_t ControlMode;
		uint16_t SourceReference;

		fp32_t Torque_max;
		fp32_t Torque_min;
		fp32_t Power_max;
		fp32_t Power_min;
		fp32_t Udcref_V_max;
		fp32_t Udcref_V_min;
		fp32_t Udc_ref_V_local;
		fp32_t Udc_ref_V_Offset;
		fp32_t PG_ref_pu;
		fp32_t k_droop_PG;
		fp32_t Speed_Quick_Stop_limit_pu;


		bool   b_Remote_Allowed;
		bool   b_IDRun_request;
		bool   b_PulseTest_request;

};
