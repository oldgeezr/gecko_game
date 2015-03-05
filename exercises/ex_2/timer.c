#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define FCPU 14000000UL

void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  //*TIMER1_CTRL = (0x02 << 24); //Prescale 64
  *TIMER1_TOP = FCPU / 8000;
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLowEnergyTimer(void)
{ 
  *CMU_OSCENCMD = (1 << 6);
  *CMU_HFCORECLKEN0 |= (1 << 4);
  *LETIMER0_CTRL |= (1 << 9) | (0x1 << 2);
  *LETIMER0_REP0 = 1;
  *CMU_LFACLKEN0 |= (1 << 2);
  *LETIMER0_TOP = 1;
  *LETIMER0_IEN = (1 << 2);
  *LETIMER0_CMD = 1;
}

void disableTimer(void)
{
  *CMU_HFPERCLKEN0 |= ~(CMU2_HFPERCLKEN0_TIMER1);
  *TIMER1_TOP = 0;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
}
