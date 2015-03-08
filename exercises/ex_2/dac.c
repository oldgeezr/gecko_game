#include "dac.h"
#include "efm32gg.h"

#define DIFFERENTIAL  (1 << 0)
#define OUTMODE_PIN   (0x1 << 4)
#define PRESC         (0x5 << 16)
#define HFPER_DAC0    (1 << 17)
#define PRSCH0        (1 << 0)

void setupDAC(uint8_t PRS)
{
  *CMU_HFPERCLKEN0 |= HFPER_DAC0;
  *DAC0_CTRL = OUTMODE_PIN | PRESC;
  *DAC0_CH0CTRL = PRSCH0;
  *DAC0_CH1CTRL = PRSCH0;

  if (PRS) {
    *DAC0_CH0CTRL |= (1 << 2);
    *DAC0_CH1CTRL |= (1 << 2);
  }
}

void disableDAC(void)
{
  *DAC0_CH0CTRL = 0;
  *DAC0_CH1CTRL = 0;
  *DAC0_CTRL = 0;
  *CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
}

void writeDAC(uint16_t data)
{
  //*DAC0_COMBDATA = data;
  *DAC0_CH0DATA = data;
  *DAC0_CH1DATA = data;
}
