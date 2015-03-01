#include <stdint.h>
#include <stdbool.h>
#include "dac.h"

#include "efm32gg.h"

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;
  *GPIO_PA_DOUTTGL = 0xffff;
}

void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
	*LETIMER0_IFC = 1;
	//*GPIO_PA_DOUTTGL = 0xffff;
	//playSine();
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
}
