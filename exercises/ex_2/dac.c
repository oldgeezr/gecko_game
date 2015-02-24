#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

static inline void writeDAC(uint16_t data) {
	//Needs to be differential
	DAC0_BASE2->DAC0_CH0DATA = data;
	DAC0_BASE2->DAC0_CH1DATA = data;
}

/*
    TODO enable and set up the Digital-Analog Converter
    
    1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
    2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL
    3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
    4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
*/

void setupDAC(void)
{
   CMU_BASE2->CMU_HFPERCLKEN0 |= (1 << 17);
   DAC0_BASE2->DAC0_CTRL = 0x50010;
   DAC0_BASE2->DAC0_CH0CTRL = 1;
   DAC0_BASE2->DAC0_CH1CTRL = 1;
}

void setupDMA(void) {
	//This is where the magic happends...
	//Format on the 8 lowest bits [A][CCCC][MMM] 

	DMA_BASE->DMA_CTRLBASE = 0b0 000 0000

	DMA_BASE->DMA_CONFIG = 1;
	DMA_BASE->DMA_CHUSEBURSTS[0] = 1;
	DMA_BASE->DMA_REQMASKC[0] = 1;
	DMA_BASE->DMA_CHALTC[0] = 1;
	DMA_BASE->DMA_CHENS[0] = 1;

	//SOURCESEL = 0b001010 -> DAC0
	//SIGSEL = 0b0000  -> DAC0 -> CH0
	DMA_BASE->DMA_CH0_CTRL = 0b001010;
}