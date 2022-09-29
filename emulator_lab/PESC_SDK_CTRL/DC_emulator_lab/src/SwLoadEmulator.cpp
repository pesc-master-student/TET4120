#ifndef SIMULINK
  #include "../include/ConfigParameters.hpp"
  extern ConfigParameters Parameters;
#endif

#include "SwLoadEmulator.hpp"


void LoadEmulator::Initialize(void){

	m_LoadType 	= static_cast<LoadType>(0);
	m_T_load_Nm = 0.0;

	// Pump Model Parameters: (Initialize your parameters in your own model)
	m_PumpData.k_L			= Parameters.Load1.Pump.k_L;
	m_PumpData.t_constant	= Parameters.Load1.Pump.t_constant;

}

fp32_t& LoadEmulator::Run(const fp32_t& Actual_Speed_rpm){

	switch (m_LoadType){
			case LoadType::Pump:
				m_T_load_Nm = RunPump(Actual_Speed_rpm);
				break;
			case LoadType::ElectricVehicle:
				m_T_load_Nm = RunPump(Actual_Speed_rpm); // Your own load
				break;
			case LoadType::ShipPropulsion:
				m_T_load_Nm = 0.3;
				break;
			case LoadType::WindPower:
				m_T_load_Nm = 0.4;
				break;
			case LoadType::HydroPower:
				m_T_load_Nm = 0.5;
				break;
			default:
				m_T_load_Nm = 0.0;
				break;
		}
	return m_T_load_Nm;
}

const fp32_t LoadEmulator::RunPump(const fp32_t ActSpeed_rpm){ // Your own load
	fp32_t t_pump_Nm;
	if(ActSpeed_rpm >= 0.0){
		t_pump_Nm = m_PumpData.k_L*ActSpeed_rpm*ActSpeed_rpm + m_PumpData.t_constant;
	}
	else {
		t_pump_Nm = -m_PumpData.k_L*ActSpeed_rpm*ActSpeed_rpm + m_PumpData.t_constant;
	}
	return t_pump_Nm;
}

/**********    IMPLEMENT YOUR LOAD MODEL HERE   *******************



*******************************************************************/


#ifdef SIMULINK // **** Only required for MATLAB/Simulink ****

const fp32_t LoadEmulator::getParam(int index){
	return param_var[index];
}

void LoadEmulator::setParamFromMask(int index, const fp32_t newParam){
	param_var[index] = newParam;
}

void LoadEmulator::setLoadType(const LoadType arg){
	m_LoadType = arg;
}

LoadType LoadEmulator::getLoadType(void){
	return m_LoadType;
}

void LoadEmulator::setSig0(const fp32_t signal) {
	input_sig0 = signal;
}

void LoadEmulator::setSig1(const fp32_t signal) {
	input_sig1 = signal;
}

fp32_t LoadEmulator::getSineWaves(void) {
	return param_var[0]*input_sig0 + param_var[1]*input_sig1 + param_var[2];
}
#endif // **** EOF required for MATLAB/Simulink ****
