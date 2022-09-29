
/************************************************************************************
::File Name: Emulator.hpp
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
1.00 aravinda.perera@ntnu.no     02.06.2020 Initial release of emulator FW/SW, auto-
                                            generated from XSG-model. Design verified
                                            using Simulink simulations.
1.10 thomas.haugan@ntnu.no  	 03.06.2020 DC-motor emulator IP-core integrated into
                                            PESC FW/SW control platform.
1.20 thomas.haugan@ntnu.no  	 04.06.2020 Added memory map. Corrected data types and
              	                            complemented variable list.

*************************************************************************************
IP-CORE MEMORY MAP:

BASEADDR: Set automatically by Vivado Design Suite during firmware design phase.
ACCESS: R = Read, W = Write, RW = Read & Write
Base address is implementation dependent while register offsets are static.

Offset	Register Name							  Access Register Content (32-bit)
------	---------------------------------------   ------ ----------------------
0x0000  emulator_wn_ts 	                          RW     scaling of rotational speed
0x0004  emulator_voltage_pu_bit 	              RW     voltage_pu_bit (volt scaling)
0x0008  emulator_trigger_selector 	              RW     tunes emulator update rate
0x000C  emulator_select_load 	                  RW     selects mechanical load type
0x0010  emulator_ra 	                          RW     armature resistance (r_a)
0x0014  emulator_polepairs 	                      RW     #pole_pairs in field winding
0x0018  emualator_kn 	                          RW     mechanical constants (k_n)
0x001C  emulator_current_pu_bit 	              RW     current_pu_bit (current scaling)
0x0020  emulator_vdc2	                          RW     converter_#2_DC_link_voltage
0x0024  emulator_vdc1	                          RW     converter_#1_DC_link_voltage
0x0028  emulator_ts_la	                          RW     armature_time_constant
0x002C  emulator_ts_tm	                          RW     mechanical time constant
0x0030  emulator_ts_tf	                          RW     field winding time constant
0x0034  emulator_tl	                              RW     constant mechanical load
0x0038  emulator_motor_id	                      R      machine identification
0x003C  emulator_n_el	                          R      electric rotational speed (n_em)
0x0040  emulator_te	                              R      electric torque (t_em)
0x0044  emulator_theta_el	                      R      electric position (theta_em)


*************************************************************************************
NOTIFICATONS:   Recommended Font: SF Mono Regular, 13.0 pt
*************************************************************************************
TODO:
- Draw ASCII-figure of emulator block with inputs/outputs

************************************************************************************/

#pragma once

#include <cmath>
#include <cstdint>
#include <types.hpp>
#include "emu_dc.h"
#include "HMI.hpp"
#include "xparameters.h"
#include <register_array.h>
#include "e_DRIVE_ID.hpp"


extern fp32_t Tsamp;
extern HMI hmi;
extern e_DRIVE_ID e_ID_DRIVE;

class Emulator {
public:


	uint32_t polepairs = 0;
	fp32_t Tstep; // Timestep in solver in FPGA
	fp32_t N_basis_rpm = 0.0;

	fp32_t A_bit_per_pu_conv1 = 0.0;
	fp32_t V_bit_per_pu_conv1 = 0.0;
	fp32_t A_bit_per_pu_conv2 = 0.0;
	fp32_t V_bit_per_pu_conv2 = 0.0;

	fp32_t Udc1_V = 0.0;
	fp32_t Udc2_V = 0.0;
	fp32_t udc1   = 0.0;
	fp32_t udc2   = 0.0;
	fp32_t Uabasis = 0.0;
	fp32_t T_L_Nm;
	fp32_t T_basis;
	fp32_t one_div_Tbasis;

	bool magnetize_topology = 0;


	fp32_t k_fNmech_Tstep = 0.0;

	struct standardData { // DC machine
		fp32_t UfN = 0.0;
		fp32_t kn = 0.0;
		fp32_t la = 0.0;
		fp32_t Tf = 0.0;
		fp32_t Tm = 0.0;
		fp32_t ra = 0.0;
		fp32_t Nn = 0.0;
		fp32_t wn = 0.0;
	} M;

	struct AXI {
		int32_t I_a_axi = 0;
		int32_t T_e_axi = 0;
		int32_t I_f_axi = 0;
		int32_t speed_axi = 0;
		int32_t theta_el_axi = 0;
		int32_t theta_mech_axi = 0;
		uint32_t Udc_1_axi = 0;
		uint32_t Udc_2_axi = 0;
		uint32_t polepairs_axi = 0;
		int32_t ra_axi = 0;
		int32_t T_L_axi = 0;
		int32_t kn_axi = 0;
		int32_t Ts_div_Tm_axi = 0;
		int32_t Ts_div_la_axi = 0;
		uint32_t k_fN_Tstep_axi = 0;
		int32_t Ts_div_Tf_axi = 0;
		bool FieldConverter_Topology = 0;
	} AXI;

	struct PU {
		fp32_t speed_pu = 0.0;
		fp32_t I_a_pu = 0.0;
		fp32_t T_e_pu = 0.0;
		fp32_t I_f_pu = 0.0;
		int32_t m_A_bit_per_pu = 0.0;
		int32_t m_V_bit_per_pu = 0.0;
		fp32_t ra_pu = 0.0;
		fp32_t T_L_pu = 0.0;
		fp32_t kn_pu = 0.0;
		fp32_t Ts_div_Tm_pu = 0.0;
		fp32_t Ts_div_la_pu = 0.0;
		fp32_t Ts_div_Tf_pu = 0.0;
		fp32_t Udc_1_pu = 0.0;
		fp32_t Udc_2_pu = 0.0;
		fp32_t theta_el = 0.0;
		fp32_t theta_mech = 0.0;
	} PU;

	void initialize(void);
	void setValues(void);
	void getValues(void);
	void setLoadTorque(const fp32_t& LoadTorque_Nm);

private:
	int32_t PU_BASE_SCALING = pow(2, 28);
	int32_t THETA_BASE_SCALING = (int32_t) pow(2, 31);
	int32_t output[8];
	int32_t mux[8];
	emu_dc m_EmulatorIP;

};
