#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define FCPU 14000000UL

/* function to setup the timer */
void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = FCPU/freq;
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

/* function to disable the timer */
void disableTimer(void)
{
  *CMU_HFPERCLKEN0 |= ~CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = 0;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0;
}
