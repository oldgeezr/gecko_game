#include "dac.h"
#include "efm32gg.h"

void setupDAC(void)
{
   *CMU_HFPERCLKEN0 |= HFPER_DAC0;
   *DAC0_CTRL = OUTMODE_PIN | PRESC;
   *DAC0_CH0CTRL = (1 << 0); //| (1 << 2);
   *DAC0_CH1CTRL = (1 << 0); //| (1 << 2);
}

void disableDAC(void)
{
  *CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
  *DAC0_CH0CTRL = 0;
  *DAC0_CH1CTRL = 0;
  *DAC0_CTRL = 0;
}

void writeDAC(uint16_t data)
{
	*DAC0_COMBDATA = data;
}
