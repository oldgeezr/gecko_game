#include <stdint.h>
#include "efm32gg.h"
#include "sounds.h"
#include "dac.h"

/*void playSine(void)
{
	static uint16_t index = 0;
	writeDAC(dma_buffer[index++]);
	if (index == DMA_BUFFER_SIZE)
	{
		index = 0;
	}
}*/

// MAYBE NOT NECESSARY TO USE A WRITE DAC FUNC OR SHOULD BE CONSEQUENT

static inline uint16_t dacOutputLevel(uint16_t amplitude, double angle, double N) {
	return (amplitude/2 + amplitude*sin(((2*PI)/N)*angle));
}

void playTone(uint16_t tone_freq)
{
	static uint16_t i = 0;
	uint16_t N = SAMPLING_FREQ/(tone_freq*10);
	*DAC0_COMBDATA = dacOutputLevel(1000,i,N);
	i = i % N;
	i++;
}

void playSquare(uint16_t tone)
{
	uint16_t N = (2*SAMPLING_FREQ)/tone;

	static uint16_t i = 0;

	if (i < N/2)
	{
		writeDAC(500);
	}
	else {
		writeDAC(0);
	}
	i %= N;
	i++;
}
