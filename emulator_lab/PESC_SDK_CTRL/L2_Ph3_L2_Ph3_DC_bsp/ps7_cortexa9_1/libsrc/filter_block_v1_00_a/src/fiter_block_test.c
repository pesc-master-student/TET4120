//-*************************************************************
// Filter_block_test.c
// Kjell Ljøkelsøy, Sintef Energy Research. 2013.
//
/// Multiplier scaling.
//			(1<< SHIFTS)
//	multiplier = --------------------------------------------------------------------------------------------------------------------
//			((SAMPLE_RATE * tidskonstant [us])/TIME_CONSTANT_SCALE_FACTOR) + 1

#include "xparameters.h"
#include "filter_block_registers.h"

#define BASEADR 			XPAR_AD_FILTER_BLOCK_BASEADDR
#define CHANNELS 			XPAR_AD_FILTER_BLOCK_NUMBER_OF_CHANNELS
#define SHIFTS				(XPAR_AD_FILTER_BLOCK_MULTIPLIER_WIDTH + XPAR_AD_FILTER_BLOCK_ACCUMULATOR_ADDITIONAL_BITS -1)
#define MULTIPLIER_MAX			((1<<(XPAR_AD_FILTER_BLOCK_MULTIPLIER_WIDTH-1))-1)
#define SAMPLE_RATE 			40000000	//[Hz]
#define TIME_CONSTANT_SCALE_FACTOR	1000000// [us/s]

int time_constant;			//[us]

int multiplier;
int shifts;
int filter_out;


//-****************************************************************************

void Filter_setup(void)
    {
    int ch_nr;
    long long ll_tmp1; // 64 bit.
    long long ll_tmp2; // Uses longlong (64 bit) temps in order to avoid overflow or roundoff errors in timeconstant calculations.

    ll_tmp1 = 1 <<  SHIFTS;
    ll_tmp2 = time_constant;

    ll_tmp2 = ll_tmp2 *  SAMPLE_RATE;
    ll_tmp2 = ll_tmp2  / TIME_CONSTANT_SCALE_FACTOR +1;
    ll_tmp1 = 	ll_tmp1 /ll_tmp2;

    // Limits multiplier value in order to avoid overflow at low time constants.
	if (ll_tmp1 > MULTIPLIER_MAX)
		{
		ll_tmp1 = MULTIPLIER_MAX;   // Imposes a minimum time constant.
		}
	if (ll_tmp1  == 0) 
		{
		ll_tmp1 = 1; // Maximum limit, avoids stalling the filter. 
		}
    multiplier = ll_tmp1;
    shifts = SHIFTS;

    for(ch_nr = 0; ch_nr < CHANNELS; ch_nr ++)
	{
	FILTER_MULTIPLIER(BASEADR,CHANNELS,ch_nr) =  multiplier;
	FILTER_NUMBER_OF_SHIFTS(BASEADR,CHANNELS,ch_nr) = shifts;
	}
    }


//-****************************************************************************

void Filter_read(void)
    {
    filter_out = FILTER_OUT(BASEADR,0);
    }

//-****************************************************************************
