/************************************************************************************
::File Name: TorqueLimiters.hpp
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
1.00 roy.nilsen@ntnu.no          16.12.2020 Original code release.

*************************************************************************************
TODO:
-

Key function for torque limiters:
	1. Speed limits
	2. DC-link voltage
	3. Current limits
	4. Power limits (Usually from external)

************************************************************************************/

#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "Constants.hpp"


#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;

struct SpeedParameters
{ //
	fp32_t 		Speed_max;
	fp32_t 		Speed_min;
	fp32_t 		dT_dSpeed;
	fp32_t 		Te_maxSpeed;
	fp32_t 		Te_minSpeed;
	fp32_t 		max;
	fp32_t 		min;
	uint16_t	inLimit;

};
struct UdcLinkParameters
{ //
	fp32_t 		Udc_max;
	fp32_t 		Udc_min;
	fp32_t 		dT_dUdc;
	fp32_t 		Te_maxUdc;
	fp32_t 		Te_minUdc;
	fp32_t 		max;
	fp32_t 		min;
	uint16_t	inLimit;
};
struct CurrentParameters
{ //
	fp32_t 		max;
	fp32_t 		min;
	uint16_t	inLimit;
};
struct PowerParameters
{ //
	fp32_t 		P_max;
	fp32_t 		P_min;
	fp32_t 		max;
	fp32_t 		min;
	uint16_t	inLimit;
};


class TorqueLimiter {
public:
	SpeedParameters 	SpeedParam;
	UdcLinkParameters	UdcParam;
	CurrentParameters	CurrentParam;
	PowerParameters		PowerParam;
	fp32_t 		Tmax_k;
	fp32_t 		Tmin_k;
	fp32_t 		Tmax_unfiltered;
	fp32_t 		Tmin_unfiltered;

	void initialize(const fp32_t InverseUdcbasis);
	const fp32_t Speed(const fp32_t torqueref,const fp32_t speed);
	const fp32_t UdcLink(const fp32_t torqueref,const fp32_t Udc, const fp32_t speed,const fp32_t Tlim_max,const fp32_t Tlim_min);
	const fp32_t Current(const fp32_t torqueref,const fp32_t currentLimStator,const fp32_t PsiMax);
	const fp32_t Power(const fp32_t torqueref,const fp32_t speed,const fp32_t PowerLimMotor, const fp32_t PowerLimGen );
	void Filter(const fp32_t torque_lim_max,const fp32_t torque_lim_min);
	void FilterSetValue(const fp32_t torque_lim_max,const fp32_t torque_lim_min);

private:

	fp32_t 		m_Tmax_k_1;
	fp32_t 		m_Tlim_increase;
	fp32_t 		m_Tmin_k_1;






};

