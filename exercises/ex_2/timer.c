#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define FCPU 14000000UL

void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_CTRL = (0x06 << 24); //Prescale 64
  *TIMER1_TOP = 10000; //should give 1 Hz
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLowEnergyTimer()
{	
	*CMU_OSCENCMD = (1 << 6);
	*CMU_HFCORECLKEN0 |= (1 << 4);
	*LETIMER0_CTRL |= (1 << 9); 
	*CMU_LFACLKEN0 |= (1 << 2);
	*LETIMER0_TOP = 1;
	*LETIMER0_IEN = 1;
	*LETIMER0_CMD = 1;
}

void disableLowEnergyTimer(void) 
{
   *CMU_HFCORECLKEN0 &= ~(1 << 4);
}

void disableTimer(void)
{
  *CMU_HFPERCLKEN0 |= ~(CMU2_HFPERCLKEN0_TIMER1);
  *TIMER1_TOP = 0;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
}
