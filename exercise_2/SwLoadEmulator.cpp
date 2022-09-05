//#include "ConfigParameters.hpp"
//extern ConfigParameters Parameters;

#include "SwLoadEmulator.hpp"


void LoadEmulator::Initialize(void){

	m_LoadType 	= static_cast<LoadType>(0);
	m_T_load_Nm = 0.0;

	// Pump Model Parameters:
	m_PumpData.k_L			= 1.0e-4; 	//Parameters.Load1.Pump.k_L;
	m_PumpData.t_constant	= 0.0; 		//Parameters.Load1.Pump.t_constant;

}

#ifdef SIMULINK
// Only required for MATLAB/Simulink:

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

fp32_t LoadEmulator::getSinus(void) {
	return input_sig0 + input_sig1;
}
// End required for MATLAB/Simulink (Can be removed or commented out when implementing in Emulator
#endif


fp32_t& LoadEmulator::Run(const fp32_t& Actual_Speed_rpm){

	switch (m_LoadType){
			case LoadType::Pump:
				m_T_load_Nm = RunPump(Actual_Speed_rpm);
				break;
			case LoadType::ElectricVehicle:
				m_T_load_Nm = getSinus();
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
				// Should we trip the drive ??
				m_T_load_Nm = 0.0;
				break;
		}
	return m_T_load_Nm;
}

const fp32_t LoadEmulator::RunPump(const fp32_t ActSpeed_rpm){
	fp32_t t_pump_Nm;
	#ifdef SIMULINK
	if(ActSpeed_rpm >=0.0){
		t_pump_Nm = m_PumpData.k_L*ActSpeed_rpm*ActSpeed_rpm + m_PumpData.t_constant;
	}
	else {
		t_pump_Nm = -m_PumpData.k_L*ActSpeed_rpm*ActSpeed_rpm + m_PumpData.t_constant;
	}
	#endif
	#ifdef ROTO
	t_pump_Nm = 1984;
	#endif

	return t_pump_Nm;
}