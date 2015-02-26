#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
  TODO calculate the appropriate sample period for the sound wave(s)
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/

#define SAMPLE_FREQ 44100

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupDAC();
void setupTimer(uint32_t period);
static inline void enterEMx(void);
static inline void waitForInterrupt(void);
static inline void setupNVIC(void);

/* Your code will start executing here */
int main(void)
{
  /* Call the peripheral setup functions */
  setupGPIO();
  setupDAC();
  setupTimer(SAMPLE_FREQ);

  /* Enable interrupt handling */
  setupNVIC();

  // Change "x" with desired EM
  enterEMx();

  // Turn OFF clk and wait for interrupt
  waitForInterrupt();

  for (;;) {};
}

static inline void enterEMx(void)
{
  *SCR = 6;
}
static inline void waitForInterrupt(void)
{
  __asm volatile ("wfi");
}

static inline void setupNVIC(void)
{
  // Enable interrupt for TIMER1 and GPIO_EVEN
  *ISER0 |= 0x802;
}
