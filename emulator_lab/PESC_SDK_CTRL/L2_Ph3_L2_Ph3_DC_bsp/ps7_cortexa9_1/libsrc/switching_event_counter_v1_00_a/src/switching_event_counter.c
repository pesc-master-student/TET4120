
//-*****************************************************************************************
// Switching_frequenc measurement
// This file contains routines for measuring switching frequency. using theswitching event counter.
// Switching_frequency is measured one channel
// This routine is run at a regular, slow update rate.
//------------------------------------------------------------------------------------------

#include "switching_event_counter_registers.h"

#include "SystemParameters.h"

 #define SAMPLEFREKVENS				 (1000000/HOVEDSLOYFEAVBRUDD_TASTETID)

// #define SWITCHING_EVENT_COUNTER_BASEADR		XPAR_switching_event_counter_0_BASEADDR
// #define SWITCHING_EVENT_COUNTER_NO_OF_CHANNELS	XPAR_switching_event_counter_0_NUMBER_OF_CHANNELS


//-----------------------------------------------------------------------------

#define COUNTER_UPDATE_RATE	20 //Hz

int switching_event_count_old[SWITCHING_EVENT_COUNTER_NO_OF_CHANNELS];

int switching_frequency_meas[SWITCHING_EVENT_COUNTER_NO_OF_CHANNELS];  //Hz.

int switching_event_counter_update_rate = SAMPLEFREKVENS /COUNTER_UPDATE_RATE;

int switching_event_interval_counter;


//-****************************************************************************

void Switching_frequency_measurement_start(void)
	{
	int i;
	for( i = 0; i < SWITCHING_EVENT_COUNTER_NO_OF_CHANNELS; i++)
		{
		switching_event_count_old[i] =  SWITCHING_EVENT_COUNTER_REG(SWITCHING_EVENT_COUNTER_BASEADR,i);
		switching_event_interval_counter = 0;
		}
	}

//-****************************************************************************

void Switching_frequency_measurement(void)
	{
	int tmp1;
	int tmp2;
	int i;

	switching_event_interval_counter++;

	if (switching_event_interval_counter >= switching_event_counter_update_rate)
		{
		switching_event_interval_counter = 0;
		for( i = 0; i < SWITCHING_EVENT_COUNTER_NO_OF_CHANNELS; i++)
			{
			// Pulses since last reading.
			tmp1 = SWITCHING_EVENT_COUNTER_REG(SWITCHING_EVENT_COUNTER_BASEADR,i);
			tmp2 = tmp1 - switching_event_count_old[i];
			switching_event_count_old[i] = tmp1;


			switching_frequency_meas[i] = (tmp2 *  SAMPLEFREKVENS) / switching_event_counter_update_rate;
			}
		}
	}

//-**********************************************************************************
