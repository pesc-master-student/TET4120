/*
 * Motor.hpp
 *
 *  Created on: 6. jan. 2021
 *      Author: rn
 */

#ifndef SRC_MOTOR_HPP_
#define SRC_MOTOR_HPP_

#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>
#include "Constants.hpp"


#include "ConfigParameters.hpp"
extern ConfigParameters Parameters;


struct MotData
{
	fp32_t 	ra;        // [pu]
	fp32_t 	la;        // [s]
	fp32_t 	Tf;        // [s]


	fp32_t 	iamax0;
	fp32_t 	ifmax0;

};


class Motor {
public:

	void initialize();
	void run();
	void SetValues();
	void getMeasurements();
	const fp32_t CurrentLimitation(const fp32_t Iamplitude);
	const fp32_t FieldCurrentLimitation(const fp32_t Iamplitude);
	MotData	 par;

private:



};





#endif /* SRC_MOTOR_HPP_ */
