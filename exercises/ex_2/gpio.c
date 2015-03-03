#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


/* Register masks */
#define SWITCH_1  0x0080
#define SWITCH_2  0x0040
#define SWITCH_3  0x0020
#define SWITCH_4  0x0010
#define SWITCH_5  0x0008
#define SWITCH_6  0x0004
#define SWITCH_7  0x0002
#define SWITCH_8  0x0001

#define LEDS_MASK  0xFF00


/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/
  *GPIO_PA_CTRL = 2;  /* set high drive strength */
  *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */
  *GPIO_PA_DOUT = 0xffff; /* turn on LEDs D4-D8 (LEDs are active low) */

  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xff;

  *GPIO_EXTIPSELL = 0x22222222;
  *GPIO_EXTIFALL = 0xff;
  *GPIO_EXTIRISE = 0xff; 
  *GPIO_IEN = 0xff;
}

void decimalToLed(uint8_t val)
{
  *GPIO_PA_DOUT = ~( ((uint16_t)  (val << 8)) & LEDS_MASK);
}



