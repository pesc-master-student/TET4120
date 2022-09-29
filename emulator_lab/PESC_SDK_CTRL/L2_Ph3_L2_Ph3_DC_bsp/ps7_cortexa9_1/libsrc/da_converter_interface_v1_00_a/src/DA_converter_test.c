// DA converter test_routines.

#include "DA_converter_registers.h"
#include "SystemParameters.h"

/*

#define DA_BASEADR								XPAR_DA_CONVERTER_INTERFACE_0_BASEADDR

#define DA_CONVERTER_WIDTH_IN					XPAR_DA_CONVERTER_INTERFACE_0_DA_CONVERTER_WIDTH
#define DA_CONVERTER_FULL_SCALE_RAW				((1<<(DA_CONVERTER_WIDTH_IN-1))-1) // Signed.
#define DA_CONVERTER_NO_OF_HW_SIGNALS_IN  		XPAR_DA_CONVERTER_INTERFACE_0_NUMBER_OF_SIGNAL_SOURCES
#define DA_CONVERTER_SCALING_SHIFTS				XPAR_DA_CONVERTER_INTERFACE_0_SCALE_SHIFTS


*/


int DA_source_hw_sw_selector;
int DA_source_selector;
	// bit 3-0:		DA A signal source nr.  
	// bit 7-4:	: 	DA B signal source nr. 
	// bit 11-8:	DA C signal source nr.  
	// bit 15-12:	DA D signal source nr.
 int  DA_A_var_address;
 int  DA_B_var_address;
 int  DA_C_var_address;
 int  DA_D_var_address;

 // 2,5V at full scale.
 int  DA_signal_full_scale_A = 2500;
 int  DA_signal_full_scale_B = 2500;
 int  DA_signal_full_scale_C = 2500;
 int  DA_signal_full_scale_D = 2500;

//-------

 int  DA_A_value;
 int  DA_B_value;
 int  DA_C_value;
 int  DA_D_value;
 
int DA_scale;

 
 
//-***************************************************************************

void DA_converter_config
	{
	DA_CONVERTER_CONFIG_REG = 0x0;  	//Registers can be updated by hardware signals
	//bit 3: D, bit 2: C, bit 1: B bit 0: A
	
	DA_OUTPUT_OFFSET(0) = 0;
	DA_OUTPUT_OFFSET(1) = 0;
	DA_OUTPUT_OFFSET(2) = 0;
	DA_OUTPUT_OFFSET(3) = 0;

	DA_scale = DA_CONVERTER_SCALING_SHIFTS; // Gives 1:1 scaling.

	DA_OUTPUT_SCALE(0) = DA_scale;
	DA_OUTPUT_SCALE(1) = DA_scale;
	DA_OUTPUT_SCALE(2) = DA_scale;
	DA_OUTPUT_SCALE(3) = DA_scale;
	}


//-****************************************************************************

void DA_converter update(void)
	{

	int tmp

	tmp =  DA_source_hw_sw_selector;
	DA_CONVERTER_CONFIG_REG = tmp;

	DA_CONVERTER_SIGNAL_SOURCE_SELECTOR = DA_source_selector;

	DA_A_value = *(volatile int*)DA_A_var_address;
	DA_B_value = *(volatile int*)DA_B_var_address;
	DA_C_value = *(volatile int*)DA_C_var_address;
	DA_D_value = *(volatile int*)DA_D_var_address;
	
	
	if ((tmp & 0x01) == 0) //0: Register 1: external signals
		{ 
		DA_OUTPUT_A =  (DA_A_value *  DA_CONVERTER_FULL_SCALE_RAW) /DA_signal_full_scale_A;
		}
	if ((tmp & 0x02) == 0) //0: Register 1: external signals 	
		{
		DA_OUTPUT_B =  (DA_B_value * DA_CONVERTER_FULL_SCALE_RAW) /DA_signal_full_scale_B;
				}
	if ((tmp & 0x04) == 0) //0: Register 1: external signals
		{
		DA_OUTPUT_C =  (DA_C_value * DA_CONVERTER_FULL_SCALE_RAW) /DA_signal_full_scale_C;
				}
	if ((tmp & 0x08) == 0) //0: Register 1: external signals
		{
		DA_OUTPUT_D =  (DA_D_value * DA_CONVERTER_FULL_SCALE_RAW) /DA_signal_full_scale_D;
		}

	}

//-****************************************************************************

