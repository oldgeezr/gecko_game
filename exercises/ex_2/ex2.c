#include <stdint.h>
#include <stdbool.h>
#include "dac.h"

#include "efm32gg.h"

#define SAMPLE_FREQ 44100

static inline void waitForInterrupt(void)
{
  __asm volatile ("wfi");
}

static inline void setupNVIC(void)
{
  // Enable interrupt for TIMER1, GPIO_EVEN, GPIO_ODD, TIMER1, LETIMER0 and DMA
  *ISER0 |= 0x802 | (1 << 0); // | (1 << 12) | (1 << 26);
  //0x802 - 0 b 1000 0000 0010 - GPIO
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
  /*
  uint16_t i = 0;
  for (i = 0; i < 8000; i++)
  {
      //dma_buffer[i] = i/8000;
  }
  */
  //setupLowEnergyTimer();
  setupTimer(0);
  //setupGPIO();
  setupPRS();
  setupDAC();
  setupDMA();
  //setupNVIC();
  //enableDeepsleep();
  waitForInterrupt();

  for (;;) {}
}
