//-****************************************************************************
// pulse_width_modulator_test_rutiner.c
//
//  Eksempelkode for pulse_width_modulator IP modul.
//-****************************************************************************

#include "SystemParameters.h"
#include "pulse_width_modulator_registers.h"

void Pulse_width_modulator_test_start(void);
void Pulse_width_modulator_test(void);


int pwm_full_scale = 2000 ;	// [bit]// Passe stor verdi. Passer bra med DA-omformerutgangen.
int frequency = 10000; // [Hz].

int pwm_a = -300;
int pwm_b = 100;
int pwm_c = 500;
int triangle_a;
//int amplitude;
/*
#define PWM_CARRIER_COUNTER_FRACTIONAL_BITS   	XPAR_DRIVER_BLOCK_PULSEWIDTH_MODULATOR_0_CARRIER_COUNTER_FRACTIONAL_BITS

#define PWM_NO_OF_CHANNELS 						XPAR_DRIVER_BLOCK_PULSEWIDTH_MODULATOR_0_NUMBER_OF_CHANNELS
#define PWM_BASEADR								XPAR_DRIVER_BLOCK_PULSEWIDTH_MODULATOR_0_BASEADDR

	
#define FpgA_SYSTEM_CLOCK_FREQUENCY 			XPAR_PROC_BUS_0_FREQ_HZ
*/
//-****************************************************************************

void Pulse_width_modulator_test_start(void)
	{
	
	//amplitude = 2* pwm_full_skala -1;
	PWM_SIGNAL_SOURCE_REG(PWM_BASEADR)	= 0xFFFFFFFF; // Processor writes references.
	PWM_REF_SYNCH_CONFIG_REG(PWM_BASEADR)	= 0x01; // Continuous update.
	PWM_INTERRUPT_REG(PWM_BASEADR)	= 0x0; // No interrupt
	PWM_AMPLITUDE_REG(PWM_BASEADR)	 = 2* pwm_full_scale -1;
	PWM_HYSTERESIS_REG(PWM_BASEADR)	 = 10;    // Some hysterese.
	PWM_INCREMENT_REG(PWM_BASEADR) = frequency * (2* pwm_full_scale -1) / ((FpgA_SYSTEM_CLOCK_FREQUENCY /2) / (1 << PWM_CARRIER_COUNTER_FRACTIONAL_BITS)) ;

	PWM_REFERENCE_REG(PWM_BASEADR,0)	 = 0;
	PWM_REFERENCE_REG(PWM_BASEADR,1)	 = 0;
	PWM_REFERENCE_REG(PWM_BASEADR,2)	 = 0;
	
	PWM_TRIANGLE_PHASE_SHIFT_REG(PWM_BASEADR,PWM_NO_OF_CHANNELS,0)	 = 0; // (0 * 2* amplitude) /3;
	PWM_TRIANGLE_PHASE_SHIFT_REG(PWM_BASEADR,PWM_NO_OF_CHANNELS,1)	 = 0; // (1* 2 * amplitude) /3;
	PWM_TRIANGLE_PHASE_SHIFT_REG(PWM_BASEADR,PWM_NO_OF_CHANNELS,2)	 = 0; // ( 2* 2* amplitude) /3;
	}

//-****************************************************************************

void Pulse_width_modulator_test(void)
	{
	PWM_REFERENCE_REG(PWM_BASEADR,0) = pwm_a;
	PWM_REFERENCE_REG(PWM_BASEADR,1) = pwm_b;
	PWM_REFERENCE_REG(PWM_BASEADR,2) = pwm_c;
	
	triangle_a = PWM_TRIANGLE_CARRIER_REG(PWM_BASEADR,PWM_NO_OF_CHANNELS,0);

	pwm_a = PWM_REFERENCE_REG(PWM_BASEADR,0);
	pwm_b = PWM_REFERENCE_REG(PWM_BASEADR,1);
	pwm_c = PWM_REFERENCE_REG(PWM_BASEADR,2);
	}
	


