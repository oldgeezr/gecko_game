#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"
#include "gpio.h"
#include "prs.h"
#include "dac.h"
#include "dma.h"
#include "timer.h"
#include "dma.h"

static inline void waitForInterrupt(void)
{
  __asm volatile ("wfi");
}

static inline void setupNVIC(void)
{
  // Enable interrupt for TIMER1, GPIO_EVEN, GPIO_ODD, TIMER1 and LETIMER1
  *ISER0 |= NVIC_GPIO_EVEN | NVIC_GPIO_ODD | NVIC_DMA | NVIC_LETIMER0;
}

static inline void enableDeepsleep(void) {
  *SCR = 6;
}

static inline void disableRamBlocks(void) {
  *EMU_MEMCTRL = RAM_BLOCK_3;
}

int main(void)
{
  //disableRamBlocks();

  setupGPIO();
  //setupLowEnergyTimer();

  setupNVIC();
  enableDeepsleep();
  waitForInterrupt();

  for (;;) {}
}
