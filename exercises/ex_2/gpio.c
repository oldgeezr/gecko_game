#include <stdint.h>
#include "efm32gg.h"
#include "gpio.h"

void setupGPIO(void)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;
  *GPIO_PA_CTRL = 2;
  *GPIO_PA_MODEH = 0x55555555;
  *GPIO_PA_DOUT = 0xffff;

  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xff;

  *GPIO_EXTIPSELL = 0x22222222;
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