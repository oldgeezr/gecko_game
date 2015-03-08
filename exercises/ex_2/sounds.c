#include <stdint.h>
#include "efm32gg.h"
#include "sounds.h"
#include "dac.h"
#include "math.h"

#define ARRLEN 10
#define SONG_LENGTH 23

uint16_t test_sounds[10] = {A, B, Bb, C, D, E, Eb, F, G, X };

uint16_t random_song[SONG_LENGTH][2] = {
  {293, 500},
  {349, 500},
  {587, 2000},
  {293, 500},
  {349, 500},
  {587, 2000},
  {659, 900},
  {698, 500},
  {659, 500},
  {698, 500},
  {659, 500},
  {523, 500},
  {440, 2000},
  {440, 2000},
  {293, 2000},
  {293, 2000},
  {349, 500},
  {440, 500},
  {440, 500},
  {293, 500},
  {349, 500},
  {391, 500},
  {329, 500}
} ;

static inline uint16_t dacOutputLevel(uint16_t amplitude, double angle, double N) {
  return (amplitude + amplitude*sin(((2*PI)/N)*angle));
}

void playStartUpSong(void)
{
  static uint16_t cnt = 0;
  static uint16_t j = 0;

  if (cnt == (random_song[j][1])) {
    cnt = 0;
    j++;
    j = j % (SONG_LENGTH - 1);
  }
  playTone(random_song[j][0]);
  cnt++;
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
