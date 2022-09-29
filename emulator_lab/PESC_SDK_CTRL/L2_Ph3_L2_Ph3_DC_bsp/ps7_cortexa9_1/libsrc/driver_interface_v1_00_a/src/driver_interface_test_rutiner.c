//-****************************************************************************
// Driver_interface_test_routines.c

//  Example code for driver_interface IP module.
//-****************************************************************************

#include "xparameters.h"
#include "driver_interface_registers.h"

void driver_interface_test_init(void);
void driver_interface_test(void);
void driver_interface_test_stop(void);

int driver_enable = 0; 
int driver_status;
int driver_ready;
int driver_status_code;

#define CLOCK_FREQ		XPAR_PROC_BUS_0_FREQ_HZ
#define DRIVER_WATCHDOG_EXPIRE_TIME	1000 	//us. Set to 2-3x normal update rate.

#define DRIVER_WATCHDOG_LOAD_VALUE	 ((CLOCK_FREQ/1000000) * DRIVER_WATCHDOG_EXPIRE_TIME)
#define BASEADR  		XPAR_DRIVER_INTERFACE_0_BASEADDR

//-****************************************************************************

void driver_interface_test_init(void)
	{
	driver_enable = 0;
	DRIVER_ENABLE_REG(BASEADR) = 0;  // Ensaures that drives are off.

	DRIVER_CONFIG_REG(BASEADR) = 0x00131313; // Reset off, a+,b+,c+: pwm, a-,b-,c- fixed on.
	DRIVER_SIGNAL_SOURCE(BASEADR) = 0x00000000 // signal source set to 0 for alle drivers.
	DRIVER_WATHCDOG_REG(BASEADR) = DRIVER_WATCHDOG_LOAD_VALUE * 4;  // some extra time in the beginning.
	}

//-****************************************************************************

void driver_interface_test(void)
	{
	driver_status = DRIVER_STATUS_REG(BASEADR);
	driver_status_code = driver_status & 0x0F;
	driver_ready  = (driver_status >> DRIVER_OK_BITNR) & 0x01;
	
	DRIVER_ENABLE_REG(BASEADR) = driver_enable; // 1 = on 0 = off.
	
	DRIVER_WATHCDOG_REG(BASEADR) = DRIVER_WATCHDOG_LOAD_VALUE; 	// Reloads watchdog timer. 
	}
	
//-****************************************************************************

void driver_interface_test_stop(void)
	{
	DRIVER_ENABLE_REG(BASEADR) = 0;  // Important! Turns off drivers when system is stopped.
	}

//-****************************************************************************

