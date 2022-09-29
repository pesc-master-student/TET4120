
/************************************************************************************
::File Name: Application_6ph_IM.hpp
::Description: missing


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
This file contains the PWM i.e. fast interrupt routine (IRQ)
::Key modules ::
	1. ADC read and scaling ( Current and DC link with PU conversion)
	2. Protection and setting up control and enable words
	3. Transformations
	4. UI / CIN model for rotor flux calculations
	5. Current Regulators d-q
	6. Dead time correction function.
	7. SPWM function call


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
#include "Drive.hpp"
#include "Converter.hpp"
#include "FirmWare_2L_3ph.hpp"
#include "RampFilter.hpp"
#include "LPFilter1.hpp"
#include "PI_controller.hpp"
//#include "CAN.hpp"

#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;


enum class ApplicationState
{
	Not_Ready_To_Switch_On				= 0x0000,
	Switch_On_Disabled			        = 0x0040,
	Ready_To_Switch_On					= 0x0021,
	Switched_On							= 0x0023,
	Operation_Enabled					= 0x0027,
	Quick_Stop_Active					= 0x0007,
	Malfunction_Reaction_Active			= 0x002F,
	Malfunction							= 0x0028,
	TestRun								= 0x0030
};


enum class ApplicationControlMode
{
	Torque								= 0,
	Speed			        			= 1,
	Power								= 2,
	DC_link_Voltage						= 3,
	LoadEmulator						= 4
};

enum class ApplicationReferenceSource
{
	WatchView							= 0,
	ModBus			        			= 1,
	HighSpeedLink						= 2,  // HSL
	Display								= 3
};


struct PLC_Data
{
	uint16_t	WatchDogTime;
	uint16_t	ControlWord;
	uint16_t	ControlMode;
	fp32_t 		torque_ref;
	fp32_t 		speed_ref;
	fp32_t 		power_ref;
	fp32_t 		Udc_V_ref;
	fp32_t 		power_max;	//
	fp32_t 		power_min;	// <=0

};

struct ApplicationControlBits
{
	uint16_t 	switch_on					    :1;    // 0
	uint16_t	voltage_enable					:1;    // 1
	uint16_t	not_quick_stop					:1;    // 2
	uint16_t	operation_enable				:1;    // 3
	uint16_t 	ramp_enable					    :1;    // 4
	uint16_t	ramp_accel					    :1;    // 5
	uint16_t	enable_set_point				:1;    // 6
	uint16_t	reset_malfunction				:1;    // 7     0->1 transition
	uint16_t 	N_8								:1;    // 8
	uint16_t    N_9								:1;    // 9
	uint16_t    remote							:1;    // 10
	uint16_t	torque_control					:1;	   // 11
	uint16_t	speed_control	       			:1;    // 12
	uint16_t	power_control    				:1;    // 13
	uint16_t	DC_link_voltage_control			:1;    // 14
	uint16_t	load_emulator					:1;    // 15
};
union uApplicationControlWord
{
	ApplicationControlBits bit;
	uint16_t  all;
};


struct ApplicationStatusBits
{
	uint16_t	ready_to_switch_on				:1;	// 0
	uint16_t	switched_on						:1;	// 1
	uint16_t	operation_enabled				:1;	// 2
	uint16_t	malfunction						:1;	// 3
	uint16_t	voltage_disabled				:1;	// 4
	uint16_t	not_quick_stop					:1;	// 5
	uint16_t	switch_on_disabled				:1;	// 6
	uint16_t	warning							:1;	// 7
	uint16_t	ready_for_operation_enable		:1;	// 8
	uint16_t	remote							:1; // 9
	uint16_t	ref_equal_actual				:1;	// 10
	uint16_t	min_max_reached					:1;	// 11
	uint16_t	torque_controlled				:1;	// 12
	uint16_t	speed_controlled				:1;	// 13
	uint16_t	power_controlled				:1;	// 14
	uint16_t	DC_link_voltage_controlled		:1;	// 15

};
union uApplicationStatusWord
{
	ApplicationStatusBits bit;
	uint16_t  all;
};

struct ApplicationFaultBits
{
	uint16_t	Time_used_Interrupt		    :1; // 0
	uint16_t	sw_Watch_Dog		    	:1; // 1
	uint16_t	Communication_Watch_Dog		:1; // 2
	uint16_t	drive						:1; // 3
	uint16_t	converter_1					:1; // 4
	uint16_t 	converter_2					:1; // 5
	uint16_t 	external_1					:1;	// 6
	uint16_t	external_2					:1;	// 7
	uint16_t    N8_14						:7; // 8-14
	uint16_t    SW_FW_mismatch				:1; // Motor SW and FPGA emulator mismatch
};
union uApplicationFaultWord
{
	ApplicationFaultBits bit;
	uint16_t  all;
};


struct ApplicationWarningBits
{
	uint16_t	Time_used_Interrupt		    :1; // 0
	uint16_t	Communication_Watch_Dog		:1; // 1
	uint16_t	drive						:1; // 2
	uint16_t	converter_1					:1; // 3
	uint16_t 	converter_2					:1; // 4
	uint16_t 	external_1					:1;	// 5
	uint16_t	external_2					:1;	// 6
	uint16_t    N7_15						:9; // 7-15

};
union uApplicationWarningWord
{
	ApplicationWarningBits bit;
	uint16_t  all;
};


struct ApplicationLimitBits
{
	uint16_t	drive						:1;	// 0
	uint16_t	converter_1					:1;	// 1
	uint16_t	converter_2					:1;	// 2
	uint16_t 	torque_control			    :1;	// 3
	uint16_t	speed_control				:1;	// 4
	uint16_t	power_control				:1;	// 5
	uint16_t	DC_link_voltage_control		:1;	// 6
	uint16_t 	torque_ref_filter  	        :1; // 7
	uint16_t	speed_ref_filter    		:1;	// 8
	uint16_t	power_ref_filter    		:1;	// 9
	uint16_t	DC_link_voltage_ref_filter  :1; // 10
	uint16_t	N11_15						:5;	// 11-15
};
union uApplicationLimitWord
{
	ApplicationLimitBits bit;
	uint16_t  all;
};



class Application {

public:
	void initialize();
	void run();

	PLC_Data    ModBus;
	PLC_Data    HSL;
	PLC_Data    Display;

private:

	ApplicationState 			m_state;
	uApplicationControlWord     m_uControlWord;
	uApplicationStatusWord		m_uStatusWord;
	uApplicationFaultWord		m_uFaultWord;
	uApplicationWarningWord		m_uWarningWord;
	uApplicationLimitWord		m_uLimitWord;
	ApplicationControlMode		m_ControlMode;
	bool						b_remote_allowed;

	ApplicationReferenceSource  m_ReferenceSource;

	void runState_Not_Ready_To_Switch_On();
	void runState_Switch_On_Disabled();
	void runState_Ready_To_Switch_On();
	void runState_Switched_On();
	void runState_Operation_Enabled();
	void runState_Quick_Stop_Active();
	void runState_Malfunction_Reaction_Active();
	void runState_Malfunction();
	void RunState_TestRun();
	void getFaultAndWarningsAndLimits();

	uDriveFaultWord 		m_DriveFaults;
	uDriveWarningWord		m_DriveWarnings;
	uDriveLimitWord			m_DriveLimits;

	PLC_Data    m_PLC;

	LpFilter1    m_TorqueRefFilter;
	RampFilter   m_TorqueRefRampFilter;
	LpFilter1    m_SpeedRefFilter;
	RampFilter   m_SpeedRefRampFilter;
	RampFilter   m_SpeedQuickStopRampFilter;
	PiController m_SpeedController;

	ValueActual m_actual_values;
	ValueRef    m_references;

	bool   		b_test_run_completed;
	bool   		b_test_run;
	bool		b_reset_malfunction_old;
	bool		b_reset_malfunction_execute;
	bool		b_reset_malfunction_completed;

	bool		b_IDRun_request;
	bool		b_PulseTest_request;


	fp32_t		m_Time_used_interrupt_Fault;
	fp32_t		m_Time_used_interrupt_Warning;
	fp32_t 		m_SpeedRef_pu;
	fp32_t 		m_SpeedRef_input_used;
	fp32_t 		m_Speed_Quick_Stop_limit_pu;
	fp32_t 		m_LoadEmualtor_Torque_test;


	bool		b_speedramp_allowed;
	bool		b_speedramp_fixed_selected;

	//CAN m_CAN;
	int can_rx_data[8];
	int can_tx_data[8];
	int can_rx = 0;
	int can_tx = 0;
	int can_status = 741;
	int can_status_Rx = 263;
	s32 can_selfTestOutput = 23;


};
