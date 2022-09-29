//---------------------------------------------------------------------------------------
// filter_block_registers.h

// #define BASE_ADDR 				XPAR_FILTER_BLOCK_BASEADR
// #define NUMBER_OF_CHANNELS 	XPAR_FILTER_BLOCK_NUMBER_OF_CHANNELS


#define FILTER_CONFIG_REG(BASE_ADDR)			*(volatile unsigned int*) (BASE_ADDR + 0)
	//	bit 2 	  Signal source: 0: Eksternal input_signals	1: Register input_signals.
	//	bit 1 	  1: Load init values into accumulator. 
	//	bit 0: 	  1: Inhibit accumulator updates.

#define FILTER_OUT_REG(BASE_ADDR,n)			*(volatile  int*) (BASE_ADDR + 4* (n + 1)) 	// Also for loading values into accumulator.  

#define FILTER_MULTIPLIER_REG(BASE_ADDR,NUMBER_OF_CHANNELS,n)			*(volatile unsigned int*) (BASE_ADDR + 4* (n+ 1 + 1* NUMBER_OF_CHANNELS))
	// FILTER_MULTIPLIER_REG(BASE_ADDR,0) is used when COMMON_PARAMETERS is set to 1. 
	// Note! Must use full width write, not bytewise write.     
	// Automatically set to 1 when when SCALING is set to fixed.
	// FILTER_MULTIPLIER_REG(b,n) = tf / (ts * 2**(FILTER_NUMBER_OF_SHIFTS_REG)+1)

#define FILTER_NUMBER_OF_SHIFTS_REG(BASE_ADDR,NUMBER_OF_CHANNELS,n)			*(volatile unsigned int*) (BASE_ADDR + 4* (n+ 1 + 2* NUMBER_OF_CHANNELS))
	//FILTER_NUMBER_OF_SHIFTS_REG(BASE_ADDR,0) is used when COMMON_PARAMETERS is set to 1.
	//Number of shifts from accumlator to input. (output is shifted up and down according to the difference between input and output width.)
	//Automatically set to max value,(MULTIPLIER_WIDTH + ACCUMULATOR_ADDITIONAL_BITS-1) when SCALING is set to fixed or multiplier. 

#define FILTER_IN_REG(BASE_ADDR,NUMBER_OF_CHANNELS,n)			*(volatile  int*) (BASE_ADDR + 4* (n+ 1 + 3* NUMBER_OF_CHANNELS))
	//	-- Register input signals. Not readable when INPUT_SOURCE_CONFIG is set to External input.
 
