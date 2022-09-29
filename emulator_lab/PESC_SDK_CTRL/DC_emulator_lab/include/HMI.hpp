
/************************************************************************************
::File Name: HMI.hpp
::Description:  Human-Machine Interface (HMI)


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


#include "ADC.hpp"
#include "DAC.hpp"
#include "GateDriver.hpp"
#include "FW_Tripping.hpp"
#include "HysteresisAmpRegulator.hpp"
#include "Measurements.hpp"
#include "Relay.hpp"
#include "Filter.hpp"
#include "Multiplexer.hpp"


class HMI {
public:

	//----- FSM -----
	struct _STATEMACHINE {
		bool clearStatusWord = false;
	} fsm;

	//----- RELAY MODULE -----
	struct _RELAY {
		 Relay::relaystatus relayOpenClose = Relay::OPEN;
		 Relay::relaystatus relayActualState;
	} relay;

	//----- ADC MODULE -----
	struct _ADC {
		int IOB_delay = 0;
		int bias = 0;
		int channel = 10;
		bool run_autocalibrate = false;
		bool run_bias = false;
	} adc;

	//----- DAC MODULE -----
	struct _DAC {
		DAC::e_Mux Mux_A;
		DAC::e_Mux Mux_B;
		DAC::e_Mux Mux_C;
		DAC::e_Mux Mux_D;

		int32_t DAC_A_sw_pregain = 1;
		int32_t DAC_B_sw_pregain = 1;
		int32_t DAC_C_sw_pregain = 1;
		int32_t DAC_D_sw_pregain = 1;

		int32_t DAC_A_out_scale = 10;
		int32_t DAC_B_out_scale = 10;
		int32_t DAC_C_out_scale = 10;
		int32_t DAC_D_out_scale = 10;
	} dac;

	//----- PWM MODULE -----
	struct _PWM {
		uint32_t sw_freq = 4000;
		fp32_t amplitude_VOLTAGE = 0.0;
		bool mode = true;
		uint32_t margin = 10;
	} pwm;


	//----- TRIPPING MODULE -----
	struct _TRIPP {
		bool clear = false;
		bool run_trim = false;
		uint32_t channel = 10;
		fp32_t ampLim = 14.00;
		fp32_t dcLim = 290.00;
	} trip;


	//----- MEASUREMENT MODULE -----
	struct _MEASURE {
		int channel = 10;
		fp32_t fineScale = 1.00;
	} SI;


	//----- LP/MA-FILTER MODULE -----
	struct _Filtering {
		uint32_t LP_usecTimeConst = 1;
		bool runMAsetScalingType = false;
		uint32_t DSP48_multi = 1;
		uint32_t DSP48_shift = 1;
		int MA_channel = 10;
		Filter::e_MuxIn inputSelect = Filter::PROGRAMMABLE_LOGIC;
		Filter::e_GAIN_MODE gainMODE = Filter::UNITY_GAIN;
	} filter;

	//----- GPIO_0 MODULE -----
	struct _Gpio_0 {
		unsigned channel = 0;
		u32 direction_mask = 0;
	} gpio_0;

	//----- EMULATOR_MODULE -----
	struct _EMULATOR_MODULE {
		// Final datatype
		fp32_t dc1_volt = 0;	// OBS SIGNED
		fp32_t dc2_volt = 0;
		uint32_t polepairs = 1;
		fp32_t t_L = 0;
		fp32_t k_n = 0.13;
		fp32_t r_a = 0.02;
		fp32_t ts_la = 0.0005;
		fp32_t ts_tm = 0.0000002;
		fp32_t ts_tf = 0.000001;


		// Test new datatype

		// Not tested

		int32_t k_fN_Tstep = 1;
		fp32_t volt_scale = 1;
		fp32_t amp_scale = 1;
		fp32_t wn_ts = 1;

		Multiplexer::e_modus MODE = Multiplexer::EMULATOR;
	} emulator;

	//----- HYSTERESIS REGULATOR_MODULE -----
	struct _AMPREGULATOR_MODULE {
		fp32_t hWidth = 0.250; // in amperes
		fp32_t amplitude_CURRENT = 0.0;
	} hysteresis_reg;


	//----- GATEDRIVER MODULE -----
	struct _GATEDRIVER {
		int32_t wtchdg_val = 16000;
		GateDriver::e_on_off enable;
		GateDriver::e_src src = GateDriver::e_src::SRC00_PWM_VOLTMODE;
		bool resetCounter = false;
	} gd;


private:
	// eMPTY

};


