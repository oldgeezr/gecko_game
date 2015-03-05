#include <stdint.h>
#include "efm32gg.h"
#include "gpio.h"

void setupGPIO(void)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;
  *GPIO_PA_CTRL = DRIVE_MODE_HIGH;
  *GPIO_PA_MODEH = PIN_MODE_PUSH_PULL_DRIVE;
  *GPIO_PA_DOUT = 0xffff;

  *GPIO_PC_MODEL = PIN_MODE_INPUT_PULL_FILTER;
  *GPIO_PC_DOUT = 0xff;

  *GPIO_EXTIPSELL = PORTSEL_PORTC;
  *GPIO_EXTIFALL = 0xff;
  *GPIO_EXTIRISE = 0xff;
  *GPIO_IEN = 0xff;
}

void setLed(uint8_t pin)
{
  *GPIO_PA_DOUT = ~( ((uint16_t)  (1 << (pin + 7))) & LED_MASK);
}

void clearLed(uint8_t pin)
{
  *GPIO_PA_DOUT = ( ((uint16_t)  (1 << (pin + 7))) & LED_MASK);
}
