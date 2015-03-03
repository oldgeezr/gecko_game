#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define SAMPLE_FREQ 44100

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupDAC();
void setupTimer(uint32_t period);
void setupLowEnergyTimer();
void setupPRS(void);
void setupDMA(void);

int main(void)
{
  setupGPIO();
  //setupPRS();
  //setupDMA();
  setupDAC();
  setupLowEnergyTimer();

  *ISER0 = 0x802 | (1 << 12) | (1 << 26);
  *SCR = 6;
  //*EMU_MEMCTRL = 7;
  __asm ("wfi");

  for (;;) {}
}
