#include <stdint.h>
#include <stdbool.h>
#include "dac.h"

#include "efm32gg.h"

#define SWITCH_1  0xfe
#define SWITCH_2  0xfd
#define SWITCH_3  0xfb
#define SWITCH_4  0xf7
#define SWITCH_5  0xef
#define SWITCH_6  0xdf
#define SWITCH_7  0xbf
#define SWITCH_8  0x7f

void __attribute__ ((interrupt)) DMA_IRQHandler()
{
	*DMA_IFC = 1;
	*DMA_CHENS = 1;
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;
  *GPIO_PA_DOUTTGL = 0xffff;
}

void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
	*LETIMER0_IFC = 1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = 0xff;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
}
