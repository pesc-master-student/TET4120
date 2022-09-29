//-****************************************************************************	
// incremental_encoder_interface_test.c

#include "SystemParameters.h" 
#include "incremental_encoder_interface_registers.h"


int enc_pos_old;
int enc_edge_time_old;
int enc_edge_time;
int enc_zero;
int enc_status;
int enc_pos;
int enc_abs_pos;
int speed;
int edges_per_rev;

int pulses_per_revolution = 2048;

#define SYNCH_IO 		enc_pos_old = ENCODER_POSITION_REG(ENCODER_BASEADR) // Dummy read to get time for freeze.
	// Using a bus synch command is better.

//-*****************************************************************************

void Encoder_config(void)
	{

	ENCODER_CONFIG_REG(ENCODER_BASEADR) = 0x001; // Resets  counters
	ENCODER_CONFIG_REG(ENCODER_BASEADR) = 0x200; // bit 9: LEDs show encoder pulses. 
	ENCODER_FILTER_REG(ENCODER_BASEADR) = 10; 	 // 100 = 1 us. at 100 MHz clock.
	ENCODER_FREEZE_CLEAR(ENCODER_BASEADR); //  Registers freeze is cleared, in case they are left frozen.
	ENCODER_FREEZE_SET(ENCODER_BASEADR); 
	SYNCH_IO;
	enc_pos_old = ENCODER_POSITION_REG(ENCODER_BASEADR);
	enc_edge_time_old = 	ENCLODER_EDGE_CAPTURE_TIMER_REG(ENCODER_BASEADR);
	enc_zero = ENCODER_ZERO_POSITION_CAPTURE_REG(ENCODER_BASEADR);
	ENCODER_FREEZE_CLEAR(ENCODER_BASEADR); 
	speed = 0;	
	edges_per_rev = 4 * pulses_per_revolution;
	}

//-****************************************************************************	

void Encoder_read(void)
	{
	int diff_time;
	int diff_pos;
	
	ENCODER_FREEZE_CLEAR(ENCODER_BASEADR);  //  Register frys settes alltid til null rett før, i tilfelle den er frosset. 
	ENCODER_FREEZE_SET(ENCODER_BASEADR); 	//  Fryser signaler, sikrer konsistens. 
	
	SYNCH_IO; // Wait until registers are frozen.


	enc_status = ENCODER_STATUS_REG(ENCODER_BASEADR);

	enc_pos = ENCODER_POSITION_REG(ENCODER_BASEADR);
	
	enc_edge_time = 	ENCLODER_EDGE_CAPTURE_TIMER_REG(ENCODER_BASEADR);
	enc_zero = ENCODER_ZERO_POSITION_CAPTURE_REG(ENCODER_BASEADR);

	ENCODER_FREEZE_CLEAR(ENCODER_BASEADR); 
	
	diff_pos  = enc_pos - enc_pos_old;
	diff_time  = enc_edge_time - enc_edge_time_old;
	
	enc_pos_old = enc_pos;
	enc_edge_time_old = enc_edge_time;
	
	if (enc_status == 0x02) // Gyldig verdi?
		{
		enc_abs_pos = enc_pos  - enc_zero;
		}
	if ( diff_time != 0) // ingen nye pulser.
		{
		speed 	= (diff_pos * (((long long)FpgA_SYSTEM_CLOCK_FREQUENCY *  60)  / diff_time )) / edges_per_rev ;
		}
	else	if (( enc_status & 0x0010) == 0x0010) // Long  time since last edge. Clock has ran out, stopped.
		{
		speed = 0;
		}
	}

//-****************************************************************************

