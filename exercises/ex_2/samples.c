#define DAC_RESOLUTION  4096
#define TONE_ARRAY_PTR  ((uint16_t *)(0x25000000))

void toneGenerator(uint16_t sample_freq, uint16_t tone_freq)
{
  uint16_t* sample_ptr = TONE_ARRAY_PTR;
  uint16_t N_SAMPLES = sample_freq/tone_freq;

  int i;
  for (i = 0; i < N_SAMPLES; i++)
  {
    *sample_ptr = (tone_freq/sample_freq)*DAC_RESOLUTION;
    sample_ptr = sample_ptr + 16;
  }

}
