
/************************************************************************************
::File Name: Drive_6ph_IM.hpp
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

Key modules
	1. ADC read and scaling ( Current and DC link with PU conversion)
	2. Protection and setting up control and enable words
	3. Transformations
	4. UI / CIN model for rotor flux calculations
	5. Current Regulators d-q
	6. Dead time correction function.
	7. SPWM function call
************************************************************************************/

#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "Constants.hpp"
#include "Converter.hpp"
#include "FirmWare_2L_3ph.hpp"
#include "PI_controller.hpp"
#include "TorqueLimiters.hpp"
#include "Motor.hpp"
#include "e_DRIVE_ID.hpp"
#include "ConfigParameters.hpp"

extern ConfigParameters Parameters;
extern const enum Multiplexer::e_modus PhysicalMode;
extern Converter Converter1;
extern Converter Converter2;
extern FirmWare FirmWare1;
extern fp32_t Tsamp;

struct DriveStatusBits {
	uint16_t	ready_to_start					:1;		// 0
	uint16_t	ready_for_operation_enable		:1;		// 1
	uint16_t	operation_enabled				:1;		// 2
	uint16_t	in_test_state					:1;		// 3
	uint16_t	warning							:1;		// 4
	uint16_t	in_fieldweakening				:1;		// 5
	uint16_t	N6_15							:10;	// 6-15

};

union uDriveStatusWord {
	DriveStatusBits bit;
	uint16_t  all;
};

struct DriveFaultBits {
	uint16_t 	Watch_Dog_conv_1			:1;	// 0
	uint16_t	Watch_Dog_conv_2			:1; // 1
	uint16_t	converter_1					:1; // 2
	uint16_t	converter_2					:1; // 3
	uint16_t	current_1					:1; // 4
	uint16_t	current_2					:1; // 5
	uint16_t 	Overspeed					:1; // 6
	uint16_t	Underspeed					:1; // 7
	uint16_t 	flux_modell					:1; // 8
	uint16_t	magnetizing					:1; // 9
	uint16_t	speed						:1; // 10
	uint16_t	encoder						:1; // 11
	uint16_t	identification				:1; // 12
	uint16_t	external_fault_1			:1; // 13
	uint16_t	external_fault_2			:1; // 14
	uint16_t	external_fault_3			:1; // 15
};

union uDriveFaultWord {
	DriveFaultBits bit;
	uint16_t  all;
};

struct DriveWarningBits {
	uint16_t 	converter_1					:1;	// 0
	uint16_t	converter_2					:1; // 1
	uint16_t	current_1					:1; // 2
	uint16_t	current_2					:1; // 3
	uint16_t	dummy4						:1; // 4
	uint16_t	dummy5						:1; // 5
	uint16_t 	Overspeed					:1; // 6
	uint16_t	Underspeed					:1; // 7
	uint16_t 	flux_modell					:1; // 8
	uint16_t	magnetizing					:1; // 9
	uint16_t	speed						:1; // 10
	uint16_t	encoder						:1; // 11
	uint16_t	identification				:1; // 12
	uint16_t	external_fault_1			:1; // 13
	uint16_t	external_fault_2			:1; // 14
	uint16_t	external_fault_3			:1; // 15
};

union uDriveWarningWord {
	DriveWarningBits bit;
	uint16_t  all;
};

struct DriveLimitBits {
	uint16_t 	converter_1					:1;	// 0
	uint16_t	converter_2    				:1;	// 1
	uint16_t	torque_ref_speed      		:1;	// 2
	uint16_t	torque_ref_power			:1;	// 3
	uint16_t 	torque_ref_udc1				:1;	// 4
	uint16_t	torque_ref_udc2				:1;	// 5
	uint16_t	torque_ref_current1			:1;	// 6
	uint16_t	torque_ref_current2			:1;	// 7
	uint16_t	N_08						:1;	// 8
	uint16_t	N_09   						:1;	// 9
	uint16_t	N_10						:1;	// 10
	uint16_t	N_11						:1;	// 11
	uint16_t	N_12						:1;	// 12
	uint16_t	N_13						:1;	// 13
	uint16_t	N_14						:1;	// 14
	uint16_t	N_15						:1;	// 15
};

union uDriveLimitWord {
	DriveLimitBits bit;
	uint16_t  all;
};

struct ValueRef {
	fp32_t 		te_ref;
	fp32_t 		te_max;
	fp32_t 		te_min;
	fp32_t 		p_max;
	fp32_t 		p_min;

	};
struct ValueActual {
	fp32_t 		te_ref_used;
	fp32_t 		te_actual;
	fp32_t 		speed;
	fp32_t 		speed_filtered;
	fp32_t 		te_max_used;
	fp32_t 		te_min_used;
};


struct Torque {
	fp32_t 		ref_input;
	fp32_t 		ref_used;
	fp32_t 		actual;
	fp32_t 		estimated;
	fp32_t 		actual_Nm;
	fp32_t 		max_used;
	fp32_t 		min_used;
	fp32_t 		max;
	fp32_t 		min;
	fp32_t 		ref_speed;
	fp32_t 		max_speed;
	fp32_t 		min_speed;
	fp32_t 		ref_udc;
	fp32_t 		max_udc1;
	fp32_t 		min_udc1;
	fp32_t 		max_udc2;
	fp32_t 		min_udc2;
	fp32_t 		ref_current;
	fp32_t 		max_Current1;
	fp32_t 		min_Current1;
	fp32_t 		max_Current2;
	fp32_t 		min_Current2;
	fp32_t 		ref_power;
	fp32_t 		max_power1;
	fp32_t 		min_power1;
	fp32_t 		max_power2;
	fp32_t 		min_power2;
};


struct Power {
	fp32_t 		mech;
	fp32_t 		mech_kW;
	fp32_t 		in;
	fp32_t 		in_kW;
	fp32_t 		dclink;
	fp32_t 		dclink_kW;
	fp32_t 		max;
	fp32_t 		min;
	fp32_t 		max_1;
	fp32_t 		min_1;
	fp32_t 		max_2;
	fp32_t 		min_2;
};

struct Voltage {
	fp32_t 		a;
	fp32_t 		f;
	fp32_t 		arefII;
	fp32_t 		dc1;
	fp32_t 		dc2;
};


struct Current {
	fp32_t 		a;
	fp32_t 		f;
	fp32_t 		a_LP;
	fp32_t 		f_LP;
	fp32_t 		a_avg;
	fp32_t 		f_avg;
	fp32_t 		a_avg_old;
	fp32_t 		f_avg_old;
	fp32_t 		a_MA;
	fp32_t 		f_MA;
	fp32_t 		limit_motor1;
	fp32_t 		limit_converter1;
	fp32_t 		limit_motor2;
	fp32_t 		limit_converter2;
};

struct Fluxlinkages {
	fp32_t 		a;
	fp32_t 		f;
};


class Drive {
public:

	enum  class DriveState
	{
		Disabled				= 0,
		Starting			    = 1,
		OperationEnabled		= 2,
		TestRun					= 3
	};

	e_DRIVE_ID m_MACHINE_ID = NoN_INITIALIZED;

	void initialize(void);
	void PreCharge(const bool Relay_on);
	void run(void);
	const uint16_t ReadyToStart(void);
	const uint16_t ReadyForOperationEnable(void);
	void RequestState(enum DriveState drive_status);
	const ValueActual GetValues(void);
	const uDriveFaultWord GetFaults(void);
	const uDriveWarningWord GetWarnings(void);
	const uDriveLimitWord GetLimits();
	void SetValues(ValueRef References);
	const uint16_t getStatus(const uint16_t conv1,const uint16_t conv2);
	void getMeasurements(void);
	const bool resetFaultWord(void);
private:
	void runState_Disabled(void);
	void runState_Starting(void);
	void runState_Operation_Enabled(void);
	void runState_Test_Run(void);
	const fp32_t TorqueRefLimitation(const fp32_t torqueref,const fp32_t tmax_extern_lim,const fp32_t tmin_extern_lim );

	/* Should be defined as CLASS
	void TorqueLimits();
	void CurrentLimits();
	void VoltageModel();
	void CurrentModel();
	void CurrentRefCalculator();
	void CurrentController();
	void FluxController();
	void FieldWeakeningController(); */

	DriveState m_state;
	uDriveStatusWord m_uStatusWord;
	uDriveFaultWord m_uFaultWord;
	uDriveWarningWord m_uWarningWord;
	uDriveLimitWord	m_uLimitWord;

	PiController    FieldCurrentController;
	PiController    ArmatureCurrentController;
	LpFilter1    	m_SpeedFilter;

	Torque    		m_te;
	Voltage  		m_u;
	Current			m_i;
	Power			m_p;
	Fluxlinkages	m_Flux;
	Motor  			m_DC;
	TorqueLimiter   m_TrefLim;

	int         m_mode;
	uint16_t    m_Bit_Status;
	ValueActual m_actualvalues;
	ValueMeasure m_conv1;
	ValueMeasure m_conv2;
	ValueConvRef m_ref_conv1;
	ValueConvRef m_ref_conv2;

	uint32_t    m_Converter1_Topology;
	fp32_t		m_Udc1_Startup_V;
	fp32_t		m_Udc2_Startup_V;
	fp32_t		m_ua_max;
	fp32_t		m_ua_fw;
	fp32_t		m_ua_fw_used;
	fp32_t		m_uf_max;
	fp32_t		m_1_div_UfN;
	fp32_t		m_1_div_UaN;
	fp32_t		m_1_div_Uabasis;
	fp32_t		m_1_div_Nrpmbasis;
	fp32_t		m_UfN;
	fp32_t		m_UaN;
	fp32_t		m_Uabasis;
	fp32_t		m_Ta;
	fp32_t		m_Tbasis_Nm;
	fp32_t		m_1_div_IfN;
	fp32_t		m_1_div_IaN;
	fp32_t		m_if_ref_pu;
	fp32_t		m_ia_ref_pu;
	fp32_t		m_psi_sref_used;
	fp32_t		m_psi_sref;
	fp32_t		m_psi_Max;
	fp32_t		m_delta_sref;
	fp32_t      m_polepairs;
	fp32_t		m_wn;
	fp32_t		m_theta_el;
	fp32_t		m_theta_adjusted;
	fp32_t		m_speed_pu;
	fp32_t      m_speed_filtered_pu;

	fp32_t		m_udc_test;
	fp32_t		m_speed_test;
	fp32_t		m_speed_test2;
	fp32_t		m_speed_max;
	fp32_t		m_speed_rate;
	fp32_t		m_speed_rate_sign;


	bool		b_Reset_Fault_Status;
	bool		b_Precharge_On_Converter1;
	bool		b_Precharge_On_Converter2;
	bool		b_Precharge_SW_Control_Converter1;
	bool		b_Precharge_SW_Control_Converter2;
	bool		b_Breaker_On_Converter1;
	bool		b_Breaker_On_Converter2;
	bool		b_Breaker_SW_Control_Converter1;
	bool		b_Breaker_SW_Control_Converter2;

	bool        b_test_index;


};

