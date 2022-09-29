#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "constants.hpp"
#include <xparameters.h>
#include "HWpar_2L_3Ph_2L_3Ph.h"
#include "basis_io_picozed7030_based_processor_board.h"
#include <pulse_width_modulator_registers.h>


class PWM_2L_3ph {
public:
	void getFWaddress(unsigned int converter_ID);
	void initialize(unsigned int converter_ID);

	const uint32_t* getSwitchingFrequency(void);
	void setSwitchingFrequency(uint32_t sw_freq_Hz);
	const int32_t* getCarrierWave(void);

	void PWM_2L_2Ph_Unipolar(const fp32_t& Ua, const fp32_t& Uf);
	void runABC(const fp32_t& m_a, const fp32_t& m_b, const fp32_t& m_c);
	const fp32_t runSpaceVector(const fp32_t& U_peak_V, const fp32_t& m_delta, const fp32_t& U_dc_V);
	const fp32_t Unipolar(const fp32_t& U_peak_V, const fp32_t& U_dc_V );
	const fp32_t UnipolarFBandHB(const fp32_t& U_FB_V,const fp32_t& U_HB_V, const fp32_t& U_dc_V);

	void setTriangleHysteresis(const uint32_t margin);
	const uint32_t getTriangleHysteresis(void);
private:
	int32_t m_pwm_a;
	int32_t m_pwm_b;
	int32_t m_pwm_c;
	int32_t m_triangle_a;
	uint32_t m_pwm_full_scale;
	uint32_t m_frequency;
	uint32_t m_pwm_base_address;
	uint32_t m_carrier_counter_fractional_bits;
	uint32_t m_pwm_number_of_channels;

	fp32_t m_M;
	fp32_t m_Mc;
	fp32_t m_m_a;
	fp32_t m_m_b;
	fp32_t m_m_c;
	fp32_t inverse_U_dc_V;
	fp32_t m_M_max;  			// Parameter to be set
	fp32_t m_U_peak_max_V;	// To be returned to Drive Layer for limiting controllers

	uint32_t triangle_margin;
};
