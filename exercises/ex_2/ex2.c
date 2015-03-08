#include "efm32gg.h"
#include "ex2.h"
#include "gpio.h"

static inline void waitForInterrupt(void)
{
  __asm volatile ("wfi");
}

static inline void setupNVIC(void)
{
  // Enable interrupt for TIMER1, GPIO_EVEN, GPIO_ODD, DMA and LETIMER1
  *ISER0 |= NVIC_GPIO_EVEN | NVIC_GPIO_ODD | NVIC_DMA | NVIC_LETIMER0;
}

static inline void enableDeepsleep(void) {
  *SCR = 6;
}

static inline void disableRamBlocks(void) {
  // Unlock EMU registers
  *EMU_LOCK = 0xADE8;
  *EMU_MEMCTRL = RAM_BLOCK_1_3;
  *EMU_LOCK = 0;
}

int main(void)
{
  disableRamBlocks();
  setupGPIO();
  setupNVIC();
  enableDeepsleep();
  waitForInterrupt();

  for (;;) {}
}
