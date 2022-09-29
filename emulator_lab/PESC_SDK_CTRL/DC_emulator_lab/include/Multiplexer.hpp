/*
 * Multiplexer.hpp
 *
 *  Created on: 26. mai 2020
 *      Author: lokaladm_thohau
 */

#pragma once

#include "Multiplexer.h"
#include "xparameters.h"

#include <types.hpp>
#include <stdlib.h>
#include <xil_io.h>


class Multiplexer {
private:

	uint32_t selector;
	uint32_t slv_register[4];
public:
	enum e_modus {EMULATOR = 0 , PHYSICAL = 1};
	void setSelector(Multiplexer::e_modus selector);
	void readSlvRegsMUX(void);
};
