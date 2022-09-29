//-****************************************************************************
// Testbench frame. 

extern void driver_interface_test_init(void);
extern void driver_interface_test(void);
extern void driver_interface_test_stop(void);

int  drift = 1;

//-****************************************************************************

main(void)
	{
	driver_interface_test_init();
	drift = 1;
	while (drift == 1) 
		{
		driver_interface_test();
		}
	driver_interface_test_stop();	
	return 0;
	}

//-****************************************************************************
