//-********************************************************************************
// incremental_encoder_interface_registers.h  
// Author:  Kjell Ljøkelsøy Sintef Energi 2009 -2017.

#define ENCODER_STATUS_REG(BASEADR)		*(volatile int*)(BASEADR + 0x0) 

	#define ENCODER_ERROR_BITNR   			19
	// bit 19-16 Error signals: bit 19: ERR, bit 18: ERR_B, bit 17. ERR_A, bit 16: ERR_0 
	// bit 10-8: Encoder input signals : bit 10: B, bit 9: A, bi 8: Z
	
	#define ENCODER_EDGE_CLOCK_RUNOUT_FLAG_BITNR	4 // 1: Edge capture timer has stopped. Long time since last edge.
	#define ENCODER_ZERO_CAPTURED_FLAG_BITNR   		1 // 1: Has captured a zero track pulse.  	
	#define ENCODER_REGISTER_FREEZE_BITNR  			0 // 1: Freeze. (Only writeable bit in status reg). 
	
	
#define ENCODER_CONFIG_REG(BASEADR)		*(volatile int*)(BASEADR + 0x4) 
	// bit 19-16 LED write_reg. bit 19: Err, bit 18: B,bit 17: A, bit 16: Z 
	// bit 9: LED fucntion. 1: encoder_signals, 0: write_reg. (Used for turning off LEDS)
	// bit 8: Direction 
	// bit 1: Reset zero_captured_flag.  
	// bit 0: Reset. 
		
#define ENCODER_FILTER_REG(BASEADR)		*(volatile int*)(BASEADR + 0x8)  
	// bit 15 - 0.  Hysteresis time before a level change propagates through filter [FpgA system clock pulses].  Max value 65535.	 100 = 1 us. at 100 MHz clock.  		
	
#define ENCODER_POSITION_REG(BASEADR)		*(volatile int*)(BASEADR + 0xC)  
	// Read pulse counter. 	Bidirectional counting. 
	
#define ENCODER_ZERO_POSITION_CAPTURE_REG(BASEADR)		*(volatile int*)(BASEADR + 0x10)  
	// Pulse counter value captured when zero pulse event coccurred. 
	
#define ENCLODER_EDGE_CAPTURE_TIMER_REG(BASEADR)		*(volatile int*)(BASEADR + 0x14)  
	// Clock pulses since last edge. 
	// This clock stops after some time after last event, in order to avoud overflow.
	
// Macros for freezing of read registers. The counters themselves are not frozen. 
// Register values must be frozen before read, in order to ensure data consistency.
	
	#define ENCODER_FREEZE_SET(BASEADR)   		ENCODER_STATUS_REG(BASEADR) = 1	
	#define ENCODER_FREEZE_CLEAR(BASEADR)  	ENCODER_STATUS_REG(BASEADR) = 0

