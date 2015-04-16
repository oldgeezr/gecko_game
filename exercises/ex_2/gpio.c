#include "efm32gg.h"
#include "gpio.h"

#define LED_MASK                    0xFF00
#define LED_DEFAULT_OUT             0x7000
#define DRIVE_MODE_HIGH             2
#define PIN_MODE_PUSH_PULL_DRIVE    0x55555555
#define PIN_MODE_INPUT_PULL_FILTER  0x33333333
#define PORTSEL_PORTC               0x22222222

void setupGPIO(void)
{
  /* Enable GPIO clock */
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

  /* Set 8 highest bits of PORTA as output pins with high drive strength */
  *GPIO_PA_CTRL     = DRIVE_MODE_HIGH;
  *GPIO_PA_MODEH    = PIN_MODE_PUSH_PULL_DRIVE;
  *GPIO_PA_DOUT     = LED_DEFAULT_OUT;

  /* Set 8 lowest bits of PORTC as input pins with pull-up */
  *GPIO_PC_MODEL    = PIN_MODE_INPUT_PULL_FILTER;
  *GPIO_PC_DOUT     = 0xff;

  /*  */
  *GPIO_EXTIPSELL   = PORTSEL_PORTC;
  *GPIO_EXTIFALL    = 0xff;
  *GPIO_EXTIRISE    = 0xff;
  *GPIO_IEN         = 0xff;
}

/* Turns on LED according to input. Accepts values 1-8 */
void setLed(uint8_t pin)
{
  *GPIO_PA_DOUT     = ~( ((uint16_t) (1 << (pin + 7))) & LED_MASK );
}

/* Remove? Not used. Find use? */
void clearLed(uint8_t pin)
{
  *GPIO_PA_DOUT     =  ( ((uint16_t) (1 << (pin + 7))) & LED_MASK );
}
