#include <stdint.h>

#include "efm32gg.h"
#include "timer.h"

/* Timer magic numbers */
#define OVERFLOW_ENABLE (1 << 0)

/* LowEnergyTimer magic numbers */
#define LFRCO_ENABLE      (1 << 6)
#define LE_ENABLE         (1 << 4)
#define LETIMER0_ENABLE   (1 << 2)
#define COMP0TOP_ENABLE   (1 << 9)
#define COMP0_VALUE        1
#define SET_START         (1 << 0)

#define SET_STOP          (1 << 1)
#define LFRCO_DISABLE     (1 << 7)

#define LETIMER_IEN_UF    (1 << 2)
#define LETIMER_IEN_COMP0 (1 << 0)



void setupTimer(uint16_t freq)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  // *TIMER1_CTRL = (0x06 << 24); //Prescale 64
  *TIMER1_TOP = FCPU/freq; //should give 1 Hz
  *TIMER1_IEN = OVERFLOW_ENABLE;
  *TIMER1_CMD = SET_START;
}

void setupLowEnergyTimer(void)
{
	*CMU_OSCENCMD      |= LFRCO_ENABLE;
  *CMU_HFCORECLKEN0  |= LE_ENABLE;
	*LETIMER0_CTRL     |= COMP0TOP_ENABLE;
	*CMU_LFACLKEN0     |= LETIMER0_ENABLE;
	*LETIMER0_COMP0     = COMP0_VALUE;
	*LETIMER0_IEN       = LETIMER_IEN_COMP0;
	*LETIMER0_CMD       = SET_START;
}

void disableLowEnergyTimer(void)
{
  *LETIMER0_CMD  =   SET_STOP;
	*CMU_OSCENCMD |=   LFRCO_DISABLE;
	*CMU_OSCENCMD &= ~(LFRCO_ENABLE);
}

void disableTimer(void)
{
  *TIMER1_IEN       = 0;
  *TIMER1_CMD       = 0;
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
}
