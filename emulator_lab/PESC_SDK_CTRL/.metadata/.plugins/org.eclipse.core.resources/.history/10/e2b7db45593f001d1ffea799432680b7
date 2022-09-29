
/************************************************************************************
::File Name: Converter_2L_3ph.hpp
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

#include <cmath>
#include <cstdint>
#include <types.hpp>
#include "PWM_2L_3ph.hpp"
#include "GateDriver.hpp"
#include "SetBit.hpp"
#include "FW_Tripping.hpp"
#include "Filter.hpp"
#include "Measurements.hpp"
#include "HysteresisAmpRegulator.hpp"
#include "HMI.hpp"

extern HMI hmi;

struct ConverterStatusBits
{
	uint16_t	Gate_Driver_ready				:1;	 // 0
	uint16_t	Gate_Driver_is_enabled			:1;	 // 1
	uint16_t	Hardware_Interlock				:1;	 // 2
	uint16_t	Main_contactor_open				:1;	 // 3
	uint16_t	OK_Ready  					    :1;	 // 4
	uint16_t	Signal_2  					    :1;	 // 5
	uint16_t	Signal_4  					    :1;	 // 6
	uint16_t	Signal_5  					    :1;	 // 7
	uint16_t	N_8_15							:8; // N_8_15

};
union uConverterStatusWord
{
	ConverterStatusBits bit;
	uint16_t  all;
};

struct ConverterFaultBits
{
	uint16_t 	Watchdog_is_expired		    :1;	// 0
	uint16_t	extern_Watchdog_expired		:1; // 1
	uint16_t	phase_A_upper				:1; // 2
	uint16_t	phase_A_lower				:1; // 3
	uint16_t	phase_B_upper				:1; // 4
	uint16_t	phase_B_lower				:1; // 5
	uint16_t 	phase_C_upper				:1; // 6
	uint16_t	phase_C_lower				:1; // 7
	uint16_t 	Brake_Chopper				:1; // 8
	uint16_t	overcurrent_GD				:1; // 9
	uint16_t	overcurrent_FW				:1; // 10
	uint16_t	DC_link_overvoltage_GD		:1; // 11
	uint16_t	DC_link_overvoltage_FW		:1; // 12
	uint16_t	overtemperature_GD			:1;	// 13
	uint16_t	DC_link_undervoltage_SW		:1;	// 14
	uint16_t	overcurrent_SW				:1;	// 15
};
union uConverterFaultWord
{
	ConverterFaultBits bit;
	uint16_t  all;
};

struct ConverterWarningBits
{
	uint16_t 	DC_link_overvoltage  	    :1;	// 0
	uint16_t 	DC_link_undervoltage  	    :1;	// 1
	uint16_t	overcurrent   		        :1; // 2
	uint16_t	overtemperature				:1; // 3
	uint16_t	N_14_15						:12;// N_4_15
};
union uConverterWarningWord
{
	ConverterWarningBits bit;
	uint16_t  all;
};


struct ValueConvRef
	{
		fp32_t 		Us_peak_V;
		fp32_t 		delta_s_s;
		fp32_t 		I_aref_A;
		fp32_t 		I_bref_A;
		fp32_t 		I_cref_A;
		fp32_t 		HysteresisWidth_A;
	};
struct ValueMeasure
{
	fp32_t 		I_sa_A;
	fp32_t 		I_sb_A;
	fp32_t 		I_sc_A;
	fp32_t 		I_sa_avg_A;
	fp32_t 		I_sb_avg_A;
	fp32_t 		I_sc_avg_A;
	fp32_t 		U_dc_V;
	fp32_t 		U_dc_avg_V;
	fp32_t 		U_a0_V;
	fp32_t 		U_b0_V;
	fp32_t 		U_c0_V;
	fp32_t 		U_a0_avg_V;
	fp32_t 		U_b0_avg_V;
	fp32_t 		U_c0_avg_V;
	fp32_t 		U_peak_max_V;
	fp32_t 		I_limit_A;

};



class Converter {
public:

	enum class ConverterState
	{
		Disabled			= 0,
		Enabled			    = 1,
		TestRun				= 2
	};

	const bool resetFaultWord(void);
	void getFWaddress(unsigned int converter_number);
	void initialize(unsigned int converter_number);
	void SetValues(ValueConvRef References);
	const uint16_t getStatus(unsigned int converter_ID);
	const ValueMeasure getMeasurements();
	const uConverterFaultWord GetFaults();
	const uConverterWarningWord GetWarnings();
	void RequestState(enum ConverterState converter_status);
	void run();
	fp32_t angle_sinus;
private:
		ConverterState m_state;
		uConverterStatusWord m_uStatusWord;
		uConverterFaultWord m_uFaultWord;
		uConverterWarningWord m_uWarningWord;

		void runState_Disabled();
		void runState_Enabled();
		void runState_Test_Run();
		const fp32_t CurrentLimitation(const fp32_t Iamplitude);

		PWM_2L_3ph m_PWM;
		GateDriver m_GD;
		SetBit m_SetBit;
		FW_Tripping m_Tripp;
		Filter m_MA_Filter;
		Filter m_LP_Filter;
		Measurements m_IU;
		ValueMeasure m_measured_values;
		HysteresisAmpRegulator m_hysteresis_ctrl;

		fp32_t m_Ia_Warning_A;
		fp32_t m_Ib_Warning_A;
		fp32_t m_Ic_Warning_A;
		fp32_t m_Udc_max_Warning_V;
		fp32_t m_Udc_min_Warning_V;
		fp32_t m_Ia_Fault_A;
		fp32_t m_Ib_Fault_A;
		fp32_t m_Ic_Fault_A;
		fp32_t m_Ia_Limit_A;
		fp32_t m_Ib_Limit_A;
		fp32_t m_Ic_Limit_A;
		fp32_t m_Udc_min_Fault_V;

		fp32_t m_Us_peak_V;
		fp32_t m_delta_s_s;
		fp32_t m_delta_s_s_deg;
		fp32_t m_U_peak_max_V;
		fp32_t m_I_aref_A;
		fp32_t m_I_bref_A;
		fp32_t m_I_cref_A;
		fp32_t m_HysteresisWidth_A;
		uint32_t m_GD_status;
		uint32_t m_Tripp_status;
		uint16_t m_status;
		int32_t m_LP_time_us = 1;
		fp32_t MA_filter_gain = 1.00;
		uint32_t m_Converter_ID;
		uint32_t m_Converter_Topology;

		bool		b_Reset_Fault_Status;
};
