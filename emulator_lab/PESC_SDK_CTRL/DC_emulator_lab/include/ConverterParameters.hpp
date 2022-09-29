
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
	1. Definition of Converter parameters. This is parameters to be read from Flash.
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

struct ConverterParameters
{

	struct ADCStruct
	{
		fp32_t bit_per_A_a = 102.4;		// To be calculated for the actual board and current sensor phase a
		fp32_t bit_per_A_b = 102.4;		// To be calculated for the actual board and current sensor phase b
		fp32_t bit_per_A_c = 102.4;		// To be calculated for the actual board and current sensor	phase c
		fp32_t bit_per_V   = 1.28;
		int32_t offset_CH_0 = 0;
		int32_t offset_CH_1 = 0;
		int32_t offset_CH_2 = 0;
		int32_t offset_CH_3 = 0; // Dc-link volt
	} ADC;

	struct FWTrippStruct
	{
		fp32_t Ia_A;			// Max and min value equal, but opposite sign
		fp32_t Ib_A;			// Max and min value equal, but opposite sign
		fp32_t Ic_A;			// Max and min value equal, but opposite sign
		fp32_t Udc_V;       	// Given by DC-link capacitor rating
	} FWTripp;

	struct FaultStruct
	{
		fp32_t Ia_A;			// Max and min value equal, but opposite sign
		fp32_t Ib_A;			// Max and min value equal, but opposite sign
		fp32_t Ic_A;			// Max and min value equal, but opposite sign
		fp32_t Udc_min_V;       // Undervoltage trip
	} Fault;

	struct WarningStruct
	{
		fp32_t Ia_A;			// Max and min value equal, but opposite sign
		fp32_t Ib_A;			// Max and min value equal, but opposite sign
		fp32_t Ic_A;			// Max and min value equal, but opposite sign
		fp32_t Udc_max_V;       // Given by DC-link capacitor rating
		fp32_t Udc_min_V;       // Undervoltage warning
	} Warning;

	struct LimitStruct
	{
		fp32_t Ia_A;			// Constant limit at time being. Should be less than  Fault limit !!!
		fp32_t Ib_A;			// Constant limit at time being. Should be less than  Fault limit !!!
		fp32_t Ic_A;			// Constant limit at time being. Should be less than  Fault limit !!!
	} Limit;

	uint32_t Converter_Topology;         // 0: 3 phase, 1: H-bridge,  (2:Buck)

};
