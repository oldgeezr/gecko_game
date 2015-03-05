#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"

void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  // *TIMER1_CTRL = (0x06 << 24); //Prescale 64
  *TIMER1_TOP = FCPU/freq; //should give 1 Hz
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLowEnergyTimer(void)
{
	*CMU_OSCENCMD |= (1 << 6);
  *CMU_HFCORECLKEN0 |= (1 << 4);
	*LETIMER0_CTRL |= (1 << 9);
	*CMU_LFACLKEN0 |= (1 << 2);
	*LETIMER0_COMP0 = 1;
	*LETIMER0_IEN = LETIMER_IEN_COMP0;
	*LETIMER0_CMD = 1;
}

void disableLowEnergyTimer(void)
{
	*LETIMER0_CTRL &= ~(1 << 9);
	*LETIMER0_COMP0 = 0;
	*LETIMER0_IEN = 0;
	*LETIMER0_CMD = 0;
	*CMU_OSCENCMD &= ~(1 << 6);
	*CMU_LFACLKEN0 &= ~(1 << 2);
	*CMU_HFCORECLKEN0 &= ~(1 << 4);
}

void disableTimer(void)
{
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
}
