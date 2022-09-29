/*
 * PI.hpp
 *
 *  Created on: 23. sep. 2019
 *      Author: raghbend
 */

#ifndef SRC_PI_CONTROLLER_HPP_
#define SRC_PI_CONTROLLER_HPP_
#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "constants.hpp"


class PiController
{
public:

	const fp32_t& Run(const fp32_t& ref,const fp32_t& measured,const fp32_t& m_feedforward, const fp32_t& limit_min,const fp32_t& limit_max);
	void SetParameters(const fp32_t& gain,const fp32_t& Ti,const fp32_t& Ta,const fp32_t& Kv, const fp32_t& t_samp);
	void SetValues(const fp32_t& output,const fp32_t& feedforward,const fp32_t& limit_min,const fp32_t& limit_max);
	bool b_InLim;

private:
	fp32_t m_Kp;
	fp32_t m_Ti;
	fp32_t m_Kv=1.0;  // Default equal 1
	fp32_t m_Ta;
	fp32_t m_prop;
	fp32_t m_integral;
	fp32_t m_integral_k;
	fp32_t m_ek;
	fp32_t m_ek1;
	fp32_t m_u_k_unsat; // without limit
	fp32_t m_u_k; // with limit
	fp32_t m_tsamp_div_Ti;
	fp32_t m_tsamp_div_Ta;

	fp32_t m_min;
	fp32_t m_max;
	fp32_t m_status; 		// -1 = lower limit, 0= no limit, 1= upper limit
	fp32_t m_feedforward;

};


#endif /* SRC_PI_CONTROLLER_HPP_ */
