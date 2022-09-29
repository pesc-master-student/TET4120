/*
 * Encoder.hpp
 *
 *  Created on: Oct 9, 2019
 *      Author: aravinda
 */

#pragma once

#include "incremental_encoder_interface_registers.h"
#include "basis_io_picozed7030_based_processor_board.h"

class Encoder {
public:
	void getFWadress(void);
	void initialize(void);
	void getValues(void);

	float m_speed;
	int m_theta;
	float theta_rad;
	float theta_abs_rad;
	bool  b_first_zero_pulse_detected;

private:
	int m_enc_adress;
	int m_pos;
	int m_pos_old;
	int m_EdgeTime_old;
	int m_EdgeTime;
	int m_zero;
	int m_status;
	int m_AbsPos;
	int m_EdgesPerRev;
	int m_PulsesPerRev = 2048;
	int diff_time;
	int diff_pos;
	int m_bitCount;
	float m_inv_EdgesPerRev;
	int m_theta_abs;
};



