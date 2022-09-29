//-*****************************************************************************
// Switching event countere registers
// Author : Kjell Ljøkelsøy Sintef Energi 2010-2017.


#define SWITCHING_EVENT_COUNTER_REG(BASE_ADDR,channel)		*(volatile int*) ( BASE_ADDR +  0 + 4 * channel)


