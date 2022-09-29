//-**********************************************************
// DA_converter_registers.h
// Author:  Kjell Ljøkelsøy Sintef Energi 2008.
// 			Kjell Ljøkelsøy Sintef Energi 2017.


//#define DA_CONVERTER_TOP			((1<<(DA_WIDTH_IN-1))-1) // Bipolsar signal. 

//---------------

#define DA_OUTPUT(channel)			*(volatile int*)(DA_BASEADR + 4 * channel) 
	// Without scaling: 12 bit. With scaling 24 bit. 	
	// Note! Values larger than 24 bit gives overflow even when saturation is used.  
	// Read gives shifted input signals when scaling is selected, raw values without. 
	
#define DA_OUTPUT_A 	DA_OUTPUT(0) 
#define DA_OUTPUT_B 	DA_OUTPUT(1)
#define DA_OUTPUT_C 	DA_OUTPUT(2)
#define DA_OUTPUT_D 	DA_OUTPUT(3)

#define DA_CONVERTER_CONFIG_REG	*(volatile int*)(DA_BASEADR + 4*4)
	// bit 3-0: 1: Registers can be updates with FPGA hardware signals.  bit 3: D, bit 2: C, bit 1: B bit 0: A 
	
#define DA_CONVERTER_SIGNAL_SOURCE_SELECTOR	*(volatile int*)(DA_BASEADR + 4*5)  
	// bit 7-0:		DA A signal source nr.  
	// bit 15-8:	DA B signal source nr. 
	// bit 23-16:	DA C signal source nr.  
	// bit 31-24:	DA D signal source nr.
	
#define DA_OUTPUT_SCALE(channel)			*(volatile int*)(DA_BASEADR +  4*(6 + channel)) 
	// 18 bit width Signed. 	
	//	 #define DA_SCALE_FACTOR (1 << DA_CONVERTER_SCALING_SHIFTS)
	//  A scale factor of 	(1 << DA_CONVERTER_SCALING_SHIFTS) gives 1:1 scaling. 
	
#define DA_OUTPUT_OFFSET(channel)			*(volatile int*)(DA_BASEADR +  4*(10+channel))

