#ifndef _SAMPLES_H_
#define _SAMPLES_H_

#define DAC_RESOLUTION  4096
#define TONE_ARRAY_PTR  ((volatile uint16_t *)(0x25000000))

void toneGenerator(uint16_t, sample_freq, uint16_t tone_freq);

#endif
