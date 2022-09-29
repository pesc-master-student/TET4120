/*
 * SWDutyCycle.hpp
 *
 *  Created on: 1. jul. 2020
 *      Author: lokaladm_thohau
 */

#pragma once


#include "xgpio.h"
#include "xparameters.h"
#include "stdint.h"
#include "HMI.hpp"

extern HMI hmi;

class GPIO {

private:
	XGpio_Config XGPIO_Config;
	XGpio XGPIO_Data;
	u32 output_data = 0;
	u32 input_data = 0;

public:
	GPIO();
	void writePin(const unsigned pin_number, const bool state);
};


