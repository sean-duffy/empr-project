#include "LPC17xx.h"
#include "rit.h"

void init_ritimer(unsigned int times)
{	
	LPC_SC -> PCONP |= (1 << 16); //power on the RIT
	disable_timer();
	LPC_RIT -> RICOMPVAL = 720000 * times;
	clear_timer_counter();
	LPC_RIT -> RICTRL = (1 << 1) | (1 << 2) | (1 << 3); // also enables halt on debug and reset counter on interrupt
}

void disable_timer()
{
	LPC_RIT -> RICTRL = 0;
}

void clear_timer_counter()
{
	LPC_RIT -> RICOUNTER = 0;
}

void clear_interrupt_flag()
{
	LPC_RIT -> RICTRL |= (1<< 0);   // Call in service routine
}
