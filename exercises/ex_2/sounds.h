#ifndef _SOUNDS_H_
#define _SOUNDS_H_

#include <stdint.h>

#define SAMPLING_FREQ 32768

#define PI (3.141592653589793)

#define A 440
#define B 494
#define Bb 466
#define C 261
#define D 294
#define E 330
#define Eb 311
#define F 350
#define G 392
#define X 3000

void playTone   (uint16_t tone_freq);
void playSquare (uint16_t tone);

#endif
