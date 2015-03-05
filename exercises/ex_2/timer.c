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
	*CMU_OSCENCMD = (1 << 6);
  *CMU_HFCORECLKEN0 |= (1 << 4);
	*LETIMER0_CTRL |= (1 << 9) | (0x1 << 2);
	*CMU_LFACLKEN0 |= (1 << 2);
	*LETIMER0_TOP = 1;
	*LETIMER0_IEN = LETIMER_IEN_UF;
	*LETIMER0_CMD = 1;
}

void disableLowEnergyTimer(void)
{
	*CMU_OSCENCMD &= ~(1 << 6);
	*CMU_HFCORECLKEN0 &= (1 << 4);
	*LETIMER0_CTRL &= (1 << 9);
	*CMU_LFACLKEN0 &= (1 << 2);
	*LETIMER0_TOP = 0;
	*LETIMER0_IEN = 0;
	*LETIMER0_CMD = 0;
}

void disableTimer(void)
{
  *CMU_HFPERCLKEN0 |= ~(CMU2_HFPERCLKEN0_TIMER1);
  *TIMER1_TOP = 0;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
}
