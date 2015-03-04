#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "prs.h"
#include "dac.h"
#include "dma.h"
#include "timer.h"


#define SAMPLE_FREQ 44100

static inline void waitForInterrupt(void)
{
  __asm volatile ("wfi");
}

static inline void setupNVIC(void)
{
  // Enable interrupt for TIMER1, GPIO_EVEN, GPIO_ODD, TIMER1 and LETIMER1
  *ISER0 |= 0x802 | (1 << 12) | (1 << 26);
}

static inline void enableDeepsleep(void) {
	*SCR = 6;
}

static inline void disableRamBlocks(void) {
	*EMU_MEMCTRL = 4;
}

int main(void)
{
  //disableRamBlocks();

  setupGPIO();
  //setupPRS();
  //setupDMA();
  setupDAC();
  setupLowEnergyTimer();

  setupNVIC();
  enableDeepsleep();
  waitForInterrupt();

  for (;;) {}
}
