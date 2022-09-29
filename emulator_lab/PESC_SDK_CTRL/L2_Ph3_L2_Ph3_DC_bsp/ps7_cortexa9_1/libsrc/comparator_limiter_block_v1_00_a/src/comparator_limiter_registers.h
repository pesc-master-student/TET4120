//-*****************************************************************************
// Comparator_limiter
// Author Kjell Ljøkelsøy Sintef Energi 2009 -2017.

// Register definitions


#define COMPARATOR_FLIPFLOP_REG(BASE_ADDR)		*(volatile int*) ( BASE_ADDR +  0) // c, b, a 
	
#define COMPARATOR_FLAG_REG(BASE_ADDR)		*(volatile int*) ( BASE_ADDR + 4 *1 ) //bit  c2, b2, a2, c1, b1, a1, c0, b0, a0

#define COMPARATOR_LIMITER_OUT_REG(BASE_ADDR,channel)		*(volatile int*) ( BASE_ADDR +  4 * (2 + channel) 

#define COMPARATOR_LIMITER_REFERENCE_REG(BASE_ADDR,NO_OF_CH,COMPS_PER_CH,channel,comp) 		*(volatile int*) (BASE_ADDR + 4 * (2 + NO_OF_CH + COMPS_PER_CH * channel +  comp))



