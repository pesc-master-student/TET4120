
#include "xparameters.h"
#include "AD_converter_serial_receiver_registers.h"

void AD_converter_serial_receiver(void);
void AD_converter_serial_receiver_iobdelay_rst(void);
void AD_converter_serial_receiver_iobdelay_up(void);
void AD_converter_serial_receiver_iobdelay_down(void);

#define AD_S_BASEADR 	XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_BASEADDR


#define IOBDELAY_STEPS	32 // 64 for Virtex5 32 for 7 series	

int AD_signal[8];
int AD_offset[8];

int AD_iobdelay = XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_SIGNAL_IOBDELAY_VALUE - XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE;

//-*************************************************************************
void AD_converter_serial_receiver_test(void)
    {
    int i;
    for (i = 0; i < 8; i++)
	{
	AD_signal[i] = AD_S_SIGNAL(AD_S_BASEADR,i);
	AD_S_OFFSET(AD_S_BASEADR,i) = AD_offset[i];
	}
    }

//-*************************************************************************
	
void AD_converter_serial_receiver_iobdelay_rst(void)
	{
	
	AD_iobdelay = XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_SIGNAL_IOBDELAY_VALUE - XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE;
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 0;
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 1 << AD_S_CONFIG_IODELAY_RESET_BITNR;
	}

//-*************************************************************************
	
void AD_converter_serial_receiver_iobdelay_up(void)
	{
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 0;
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 1 << AD_S_CONFIG_IODELAY_UP_BITNR;
	AD_iobdelay ++;
	if (AD_iobdelay >= IOBDELAY_STEPS-XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE)
		{
		AD_iobdelay = - XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE;
		}
	}

//-*************************************************************************
	
void AD_converter_serial_receiver_iobdelay_down(void)
	{
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 0;
	AD_S_CONFIG_REGISTER(AD_S_BASEADR) = 1 << AD_S_CONFIG_IODELAY_DOWN_BITNR;
	AD_iobdelay --;
	if (AD_iobdelay < (-XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE) )
		{
		AD_iobdelay = IOBDELAY_STEPS-XPAR_AD_CONVERTER_SERIAL_RECEIVER_0_AD_CLOCK_IOBDELAY_VALUE-1;
		}
	}

//-*************************************************************************


