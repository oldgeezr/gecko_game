#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define FCPU 14000000UL

/* function to setup the timer */
void setupTimer(uint16_t freq)
{
  // Enable HFCLK for TIMER1
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  // Set TOP value for OF
  *TIMER1_TOP = FCPU/freq;
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLowEnergyTimer(uint16_t freq)
{
  // Enable HFCLK for LETIMER0
	*CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_LE;
  // Enable LFRCO for LETIMER0
	*CMU_OSCENCMD = CMU_OSCENCMD_LFRCOEN;
  // Enable LFCLK for LETIMER0
	*CMU_LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0;
  // Enable interrupt on compare underflow
	*LETIMER0_CTRL |= LETIMER0_CTRL_COMP0TOP;
  // Set compare TOP value
	*LETIMER0_COMP0 = FCPU/freq;
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
