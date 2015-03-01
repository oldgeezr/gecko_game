#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "samples.h"

// Might not be necessary because of PRS
/* LETIMER0 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
  uint16_t* sample_ptr = TONE_ARRAY_PTR;
  *LETIMER0_IFC = 1;
  // Put sample on DAC and divide by 2 (health insurance)
  *DAC0CH0DATA = *sample_ptr;
  *DAC0CH1DATA = *sample_ptr;
  if (sample_ptr != TONE_ARRAY_END_PTR) {
    sample_ptr++;
  } else {
    sample_ptr = TONE_ARRAY_STRT_PTR;
  }
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;

  // Put sample on DAC and divide by 2 (health insurance)
  /*
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
