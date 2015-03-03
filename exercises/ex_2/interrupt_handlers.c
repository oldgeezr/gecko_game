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

void enableLowEnergyTimer(void) 
{
  *CMU_HFCORECLKEN0 |= (1 << 4);
}

void disableLowEnergyTimer(void) 
{
	*CMU_OSCENCMD &= ~(1 << 6);
	*CMU_HFCORECLKEN0 &= (1 << 4);
	*LETIMER0_CTRL &= (1 << 9);
	*CMU_LFACLKEN0 &= (1 << 2);
	*LETIMER0_TOP = 0;
	*LETIMER0_IEN = 0;
	*LETIMER0_CMD = 0;
}


uint16_t global_freq = A;

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
	//playSquare(440);
	playTone(global_freq);
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = 0xff;

	if (*GPIO_PC_DIN != 0xff)
	{
		setupLowEnergyTimer();
		switch(*GPIO_PC_DIN) 
		{
			case SWITCH_1:
				global_freq = A;
				decimalToLed(1);
				break;
			case SWITCH_3:
				global_freq = B;
				decimalToLed(3);
				break;
			case SWITCH_5:
				global_freq	= C;
				decimalToLed(5);
				break;
			case SWITCH_7:
				global_freq = D;
				decimalToLed(7);
				break;
		}
	} else {
		disableLowEnergyTimer();
		*GPIO_PA_DOUT = 0xffff;
	}
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	//*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);

	if (*GPIO_PC_DIN != 0xff) {
		setupLowEnergyTimer();
		switch(*GPIO_PC_DIN) {
			case SWITCH_2:
				global_freq = E;
				decimalToLed(2);
				break;
			case SWITCH_4:
				global_freq = F;
				decimalToLed(4);
				break;
			case SWITCH_6:
				global_freq = G;
				decimalToLed(6);
				break;
			case SWITCH_8:
				global_freq = Eb;
				decimalToLed(8);
				break;
		}
	} else {
		disableLowEnergyTimer();
		*GPIO_PA_DOUT = 0xffff;
	}
}
