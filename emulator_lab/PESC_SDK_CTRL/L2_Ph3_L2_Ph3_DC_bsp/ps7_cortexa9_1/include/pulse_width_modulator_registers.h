//-****************************************************************************
// pulse_width_modulator_registers.h .		// Register definitions. 
// Author: Kjell Ljøkelsøy Sintef Energi 2009-2017.
//-****************************************************************************

//#define BASE_ADDR  			PWM_0_BASEADDR
//#define PWM_FRACTIONAL_BITS 		PWM_0_CARRIER_COUNTER_FRACTIONAL_BITS
//#define NO_OF_CHANNELS 		PWM_0_NUMBER_OF_CHANNELS

#define PWM_SIGNAL_SOURCE_REG(BASE_ADDR)			*(volatile unsigned int*) (BASE_ADDR + 0x04 * 0)
		// 4 bit pr. channel. --	Signal sources nr. 0-14:  hardware source, 15:  Bus register as signal source.
		// bit 3-0 channel A, -- channel above channel H.
		// bit 7-4 channel B - -- etc

	
#define PWM_REF_SYNCH_CONFIG_REG(BASE_ADDR)					*(volatile unsigned int*) (BASE_ADDR + 0x04 *1)
		// bit 0:		-- Continuous update. No latching.
		// bit 1:		-- synch_in signal
		// bit 2:		-- Triangle bottom
		// bit 3:		-- Triangle top
		// bit 4:		-- Base counter start

#define PWM_INTERRUPT_REG(BASE_ADDR)					*(volatile unsigned int*) (BASE_ADDR + 0x04 * 2)
		// bit t-1   -  0: -- Triangle bottom, triangle nr.	t: number of triangles
		// bit 2t-1  -  t: -- Triangle top, triangle nr.	t: number of triangles
		// bit 2*t		-- Base counter start
	 
#define PWM_INTERRUPT_DIVISOR_REG(BASE_ADDR)		*(volatile unsigned int*) (BASE_ADDR + 0x04 * 3)
		// Number of interrupt-source pulses pr interrupt trigger pulse ut.  Sets the interrupt-frquency as a fraction of the interrupt source frequency.
	 
#define PWM_AMPLITUDE_REG(BASE_ADDR)					*(volatile unsigned int*) (BASE_ADDR + 0x04 * 4)
		// peak to peak value.  Should be set to a value that gives sufficient headroom for overflow, a factor 3-10 times less than range for signal in.

#define PWM_INCREMENT_REG(BASE_ADDR)				*(volatile unsigned int*) (BASE_ADDR + 0x04 * 5)
		// Counter increment per FpgA system clock pulse. Note: Counts fractional bits. 1,0 =  (1<< PWM_FRACTIONAL_BITS).
	
#define PWM_HYSTERESIS_REG(BASE_ADDR)				*(volatile unsigned int*) (BASE_ADDR + 0x04 * 6)
		 // Margin between turn on and turn off.
		 // Should be large enough to suppress spuripus switching due to noise when reference follows the triangle carrier.
		 // Hysteresis should be larger than reference update rate, if not synchronised.
	
#define	PWM_BLOCK_START			8
 	
#define PWM_REFERENCE_REG(BASE_ADDR,n)		*(volatile  int*) (BASE_ADDR + 0x04 * (n + PWM_BLOCK_START ))
	// Reads actual active value, writes to a buffer register.
	// NB! Ref value equal to PWM amplitude value gives 99,999% duty cycle.
	
//#define PWM_REFERENCE_A 		PWM_REFERENCE_REG(BASE_ADDR,0)
//#define PWM_REFERENCE_B 		PWM_REFERENCE_REG(BASE_ADDR,1)
//#define PWM_REFERENCE_C		PWM_REFERENCE_REG(BASE_ADDR,2)

#define PWM_TRIANGLE_CARRIER_REG(BASE_ADDR,NO_OF_CHANNELS,n)	*(volatile  int*) (BASE_ADDR + 0x04 * (n +  PWM_BLOCK_START + 1 * NO_OF_CHANNELS))

//#define PWM_TRIANGLE_A		PWM_TRIANGLE_CARRIER_REG(BASE_ADDR,NO_OF_CHANNELS,0)
//#define PWM_TRIANGLE_B		PWM_TRIANGLE_CARRIER_REG(BASE_ADDR,NO_OF_CHANNELS,1)
//#define PWM_TRIANGLE_C		PWM_TRIANGLE_CARRIER_REG(BASE_ADDR,NO_OF_CHANNELS,2)

#define PWM_TRIANGLE_PHASE_SHIFT_REG(BASE_ADDR,NO_OF_CHANNELS,n) *(volatile unsigned int*) (BASE_ADDR + 0x04 * (n + PWM_BLOCK_START + 2 * NO_OF_CHANNELS))
			// Phase shifting of triangle signal. Value equal to PWM_AMPLITUDE_REG gives 180 graders phase shift.
			// Note: For configuration only. Not intended used for phase modulation control.
//#define PWM_TRIANGLE_PHASE_SHIFT_A			PWM_TRIANGLE_PHASE_SHIFT_REG(BASE_ADDR,NO_OF_CHANNELS,0)
//#define PWM_TRIANGLE_PHASE_SHIFT_B			PWM_TRIANGLE_PHASE_SHIFT_REG(BASE_ADDR,NO_OF_CHANNELS,1)
//#define PWM_TRIANGLE_PHASE_SHIFT_C			PWM_TRIANGLE_PHASE_SHIFT_REG(BASE_ADDR,NO_OF_CHANNELS,2)
