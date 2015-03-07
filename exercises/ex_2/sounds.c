#include <stdint.h>
#include "efm32gg.h"
#include "sounds.h"
#include "dac.h"
#include "math.h"
#include "song.h"
//#include "battle003.h"

static inline uint16_t dacOutputLevel(uint16_t amplitude, double angle, double N) {
	return (amplitude + amplitude*sin(((2*PI)/N)*angle));
}

void playSongArray() {

	static int i = 0;
	//static uint32_t SONG_LENGTH = sizeof(battle003);
  uint8_t sound = SONG1[i];
  //uint8_t sound = battle003[i];
  *DAC0_CH0DATA = (sound << 2);
  *DAC0_CH1DATA = (sound << 2);
	i = i % (SONG1_LENGTH-1);
	i++;

}

void playTone(uint16_t tone_freq)
{
	static uint16_t i = 0;
	uint16_t N = SAMPLING_FREQ/(tone_freq*10);
	writeDAC(dacOutputLevel(250,i,N));
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
