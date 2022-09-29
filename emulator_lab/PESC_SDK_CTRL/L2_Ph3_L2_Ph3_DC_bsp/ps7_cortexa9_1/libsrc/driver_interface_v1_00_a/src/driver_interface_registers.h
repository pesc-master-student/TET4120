//-****************************************************************************
//  driver_interface_registers.h .		// Register definitions. 
//  Kjell Ljøkelsøy Sintef Energi 2010 - 2016.
//-****************************************************************************

//#define BASEADR  		XPAR_DRIVER_INTERFACE_0_BASEADDR

#define DRIVER_STATUS_REG(BASEADR)					*(volatile unsigned int*) (BASEADR + 0x04 * 0)	
	// Bit 3-0:		Driver status code signal.  
	#define DRIVER_OK_BITNR							8 // 1: ready 0: not ready.
	// Bit 21-16:	Driver signals: C+, C-, B+, B- A+, A-.
	#define RESULTING_DRIVER_ENABLE_SIGNAL_BITNR	24  // Signal going out to drivers.
	#define DRIVER_ENABLE_REGISTER_VALUE_BITNR		25	// Read only.
	#define DRIVER_WATCHDOG_EXPIRED_BITNR 			26  // 1: Watchdog expired.
	#define DRIVER_RESET_STATUS_BITNR 				27  // 1: reset.
	#define DISABLE_IN_SIGNAL_BIT_STARTNR 			28	// Up to four bits.
						
#define DRIVER_ENABLE_REG(BASEADR)			*(volatile unsigned int*) (BASEADR + 0x04 * 1)	
	// Bit 0:		Inverter Enable signal register.  1: On (enable). 0: Off. 

#define DRIVER_CONFIG_REG(BASEADR)			*(volatile unsigned int*) (BASEADR + 0x04 * 2)
	// 2 bit pr driver: 	0: Permanently off. 1: Signal, 2: Inverted signal 3: Permanently on. 
	// Bit 1-0:   	driver A-
	// Bit 5-4:   	driver A+ 
	// Bit 9-8:   	driver B-
	// Bit 13-12  	driver B+
	// Bit 17-16  	driver C- 
	// Bit 21-20  	driver C+ 
	#define 		DRIVER_RESET_BITNR			27	// 1: Reset, 0: normal operation.	
		
#define DRIVER_SIGNAL_SOURCE(BASEADR)		*(volatile unsigned int*) (BASEADR + 0x04 * 3)	
	// Input selector, multiplexer. 4 bit pr. driver. 
	//bit 23-20  CH, bit 19-16 CL, bit 15- 12 BH, bit 11-8: BL bit 7-4: AH, bit 3-0 AL
	
#define DRIVER_WATHCDOG_REG(BASEADR)		*(volatile unsigned int*) (BASEADR + 0x04 * 4)	
	// Counts down to zero, blocks driver when zero.  
	// Reload value to this register regularly. 
	// Must be written as a whole word.
