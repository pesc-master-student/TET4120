#pragma once

#include <types.hpp>
#include <cmath>
#include <cstdint>

struct LoadParameters
{
	uint16_t LoadType;

	struct PumpStruct
	{
		fp32_t k_L;
		fp32_t t_constant;
	}Pump;

	struct ElectricVehicleStruct
		{
			fp32_t Kp;			                // Gain in pu for speed controller
			fp32_t Ti;			                // Integral time constant [s]
			fp32_t Ta;			                // Filter time constant (not in use)
			fp32_t Kv;			                // Feedforward gain. Usually 0 or 1 in our applications

		}EV;

	struct ShipPropulsionStruct
		{
			fp32_t Kp;			                // Gain in pu for speed controller
			fp32_t Ti;			                // Integral time constant [s]
			fp32_t Ta;			                // Filter time constant (not in use)
			fp32_t Kv;			                // Feedforward gain. Usually 0 or 1 in our applications

		}Ship;

	struct WindPowerStruct
		{
			fp32_t Kp;			                // Gain in pu for speed controller
			fp32_t Ti;			                // Integral time constant [s]
			fp32_t Ta;			                // Filter time constant (not in use)
			fp32_t Kv;			                // Feedforward gain. Usually 0 or 1 in our applications

		}Wind;

	struct HydroPowerStruct
		{
			fp32_t Kp;			                // Gain in pu for speed controller
			fp32_t Ti;			                // Integral time constant [s]
			fp32_t Ta;			                // Filter time constant (not in use)
			fp32_t Kv;			                // Feedforward gain. Usually 0 or 1 in our applications

		}Hydro;

};
