//-************************************************************
// AD_converter_serial_receiver_registers.h
// Author: Kjell Ljøkelsøy Sintef Energy Research 2008-2016.


#define AD_S_SIGNAL(BASEADDR,CHANNEL)	*(volatile int*)((BASEADDR) + 0x0 + 4*(CHANNEL))  // Received value value plus offset.

#define AD_S_OFFSET(BASEADDR,CHANNEL)	*(volatile int*)((BASEADDR) + 0x20 + 4*(CHANNEL)) // Offset value is added to received signal.

#define AD_S_CONFIG_REGISTER(BASEADDR)	*(volatile int*)((BASEADDR) + 0x40) 


	#define AD_S_CONFIG_BLOCK_NEW_VALUE_FLAG_BITNR		0	// 1: Blocks the Signal_out new flag.
	#define AD_S_CONFIG_BLOCK_SIGNAL_UPDATES_BITNR		1	// 1: Freezes ad register value update.
	#define AD_S_CONFIG_STARTUP_BLOCK_IS_ACTIVE_BITNR 	4	// 1: Startup blocking is active. ( If this flasg is stuck on the AD clock as failed.) 
	#define AD_S_CONFIG_IODELAY_RESET_BITNR 			8 	// 0->1: Resets IO delay to default value.
	#define AD_S_CONFIG_IODELAY_UP_BITNR 				9	// 0->1: Increments IO delay one notch.
	#define AD_S_CONFIG_IODELAY_DOWN_BITNR 				10	// 0->1: Decrements IO delay one notch.

