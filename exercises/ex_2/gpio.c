#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


/* Register masks */
#define SWITCH_1  0x8000
#define SWITCH_2  0x4000
#define SWITCH_3  0x2000
#define SWITCH_4  0x1000
#define SWITCH_5  0x0800
#define SWITCH_6  0x0400
#define SWITCH_7  0x0200
#define SWITCH_8  0x0100

#define LEDS_MASK  0x00FF


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
  *GPIO_PA_DOUT = ~( ((uint16_t) val) & LEDS_MASK );
}



