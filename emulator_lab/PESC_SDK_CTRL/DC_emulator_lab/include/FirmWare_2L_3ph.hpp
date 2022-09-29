
/************************************************************************************
::File Name: FirmWare_2L_3ph.hpp
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
1.00 roy.nilsen@ntnu.no          01.01.2018 Original code release.
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

#include "Converter.hpp"
#include <types.hpp>
#include "ADC.hpp"
#include "Relay.hpp"
#include "Emulator.hpp"
#include "Encoder.hpp"
#include "../src/SwLoadEmulator.hpp"
#include "HMI.hpp"
#include "Multiplexer.hpp"
#include "EnDat22.hpp"

extern HMI hmi;
extern DAC m_DAC;
extern const enum Multiplexer::e_modus PhysicalMode;

class FirmWare {
public:
	FirmWare(void);
	void getFWaddress(void);
	void initialize(void);
	void run(void);
	const fp32_t getSpeed(const bool Hardware_Mode);
	const fp32_t getPosition(const bool Hardware_Mode);
	const fp32_t getAbsPosition(const bool Hardware_Mode);
	const bool   getEncoderIndexPulse(const bool Hardware_Mode);
	const fp32_t getTorque(const bool Hardware_Mode);
	const fp32_t getLoadEmulatorTorque(const fp32_t& Speed_rpm);

private:
	ADC				m_ADC;
	Relay   		m_Relay;
	Emulator 		m_Emulator;
	Encoder      	m_Encoder;
	LoadEmulator 	m_LoadEmulator;
	Multiplexer  	m_MUX;
	EnDat22 		m_eNDAT22;
	fp32_t			m_Speed_rpm;
	fp32_t			m_TorqueLoad_Nm;
};
