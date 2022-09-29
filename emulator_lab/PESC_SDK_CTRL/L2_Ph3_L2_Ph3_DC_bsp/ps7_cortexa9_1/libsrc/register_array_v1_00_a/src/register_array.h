//---------------------------------------------------------------------------------------
// register_array.h

// #define BASE_ADDR 				XPAR_REGISTER_ARRRAY_BASEADR



#define REGISTER_ARRAY_U(BASE_ADDR,reg)		*(volatile unsigned int*) (BASE_ADDR + 4 * reg )

#define REGISTER_ARRAY(BASE_ADDR,reg)			*(volatile int*) (BASE_ADDR + 4 * reg )
