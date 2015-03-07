#include <stdint.h>
#include <stdio.h>

#define SAMPLE_FREQ 44100
#define N_TONES 28

int main(void) {

  char tone[N_TONES] =      {'C','F','G','A', 'C','F','G','A','D','F','G','A','D','F','G','A','B','E','F','G','B','E','F','G','F','G','F','F'};
  int tone_freq[N_TONES] =  {262,87,196,440,  262,87,196,440, 147,87,196,440, 147,87,196,440, 246,330,87,196, 246,330,87,196, 87,196,87,46};

  int volum = 1024;

  FILE *f = fopen("song_generator_output.txt", "w");
  if (f == NULL)
  {
        printf("Error opening file!\n");
        //exit(1);
  }

  fprintf(f, "static const uint16_t GERUDO[SONG_LENGTH] = {");

  int TOT_LENGTH = 0;

  int j;
  for (j = 0; j < N_TONES; j++) {
    int step = (tone_freq[j]*volum)/SAMPLE_FREQ;
    int length = SAMPLE_FREQ/tone_freq[j];
    TOT_LENGTH += length;

    //fprintf(f, "#define %c_LENGTH %d\n", tone[j], tone_freq[j]);
    //fprintf(f, "static const uint16_t %c[%c_LENGTH] = {,", tone[j], tone[j]);
    int i;
    for (i = 0; i < length; i++) {
      int v_level = step*i;
      fprintf(f, "%d", v_level);
      fprintf(f, ",");
    }

    for (i = length; i > 0; i--) {
      int v_level = step*i;
      fprintf(f, "%d", v_level);
      if (i > 1) {
        fprintf(f, ",");
      }
    }
  }

  fprintf(f, ",};");
  fprintf(f, "\n\n");
  fprintf(f, "#define SONG_LENGTH %d\n", TOT_LENGTH*2);

  fclose(f);
  return 0;
}
