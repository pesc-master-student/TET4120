#ifndef LOAD_EMULATOR_HPP_INCLUDED
#define LOAD_EMULATOR_HPP_INCLUDED

#include <cstdint>
#include <stdint.h>
#include <cmath>

#define _PARAMS(paramlist)		paramlist

#ifdef SIMULINK // **** Only required for MATLAB/Simulink ****
  // Included in types.hpp
  typedef unsigned short		uint16_t;
  typedef float				fp32_t;
  typedef double				fp64_t;
  typedef fp64_t				fp_t;

  // Included in Constants.hpp
  const fp32_t M_2PI              = 2.0 * M_PI;
  const fp32_t M_2PI_DIV_3 		= M_2PI/3;
  const fp32_t M_4PI_DIV_3		= 4.0/3.0 * M_PI;
  const fp32_t M_PI_DIV_2		    = M_PI/2.0;
  const fp32_t M_SQRT_1_DIV_3 	= 1/sqrt(3.0);
  const fp32_t M_1_DIV_6  		= 1.0/6.0;
  const fp32_t M_1_DIV_3  		= 1.0/3.0;
#endif // **** EOF required for MATLAB/Simulink ****

enum class LoadType
{
	Pump							= 0,
	ElectricVehicle			        = 1,
	ShipPropulsion					= 2,
	WindPower						= 3,
	HydroPower						= 4
};

struct PumpDataStruct
{
	fp32_t k_L;
	fp32_t t_constant;
};


class LoadEmulator
{
public:
	void Initialize(void);
	fp32_t& Run(const fp32_t& ActualSpeed_pu);

	#ifdef SIMULINK // **** Only required for MATLAB/Simulink ****
	  LoadType getLoadType(void);
	  void setLoadType(const LoadType arg);
	  void setParamFromMask(int index, const fp32_t newParam);
	  const fp32_t getParam(int index);
	  void setSig0(const fp32_t signal);
	  void setSig1(const fp32_t signal);
	  fp32_t getSineWaves(void);
	#endif // **** EOF required for MATLAB/Simulink ****
private:
	const fp32_t RunPump(const fp32_t ActSpeed_pu);

	LoadType 		m_LoadType;
	PumpDataStruct 	m_PumpData;

	fp32_t m_T_load_Nm;

	bool   b_error_max;
	bool   b_deviation_max;
	bool   b_in_limit;
	bool   b_input_reached;

    #ifdef SIMULINK // **** Only required for MATLAB/Simulink ****
	  fp32_t param_var[3];
	  fp32_t input_sig0;
	  fp32_t input_sig1;
    #endif // **** EOF required for MATLAB/Simulink ****

};

#endif //\#ifndef LOAD_EMULATOR_HPP_INCLUDED
