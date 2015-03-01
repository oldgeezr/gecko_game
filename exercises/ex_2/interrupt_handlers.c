#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

// Might not be necessary because of PRS
/* LETIMER0 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
  *LETIMER0_IFC = 1;
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;

  // Put sample on DAC and divide by 2 (health insurance)
  /*
  sample = sample/2;
  *DAC0CH0DATA = sample;
  *DAC0CH1DATA = sample;
  */
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
}
