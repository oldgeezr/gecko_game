#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define CPU_F 14000000UL

/* function to setup the timer */
void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = CPU_F/freq;
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLowEnergyTimer(uint16_t freq)
{
	*CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_LE;
	*CMU_OSCENCMD = CMU_OSCENCMD_LFRCOEN;
	*CMU_LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0;
	*LETIMER0_CTRL |= LETIMER0_CTRL_COMP0TOP;
	*LETIMER0_COMP0 = CPU_F/freq;
	*LETIMER0_IEN = 1;
	*LETIMER0_CMD = 1;
  // Route the underflow flag from LETIMER to PRS
  *LETIMER0_ROUTE = LETIMER0_ROUTE_LET0_O0;
}

/* function to disable the timer */
void disableTimer(void)
{
  *CMU_HFPERCLKEN0 |= ~CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = 0;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
}
