#include "interrupt_handlers.h"
#include "dac.h"
#include "sounds.h"
#include "timer.h"
#include "gpio.h"
#include "prs.h"
#include "dma.h"
#include "efm32gg.h"

static uint16_t global_freq = A;
static uint8_t STARTUPSONG = 0;

/*  */
void __attribute__ ((interrupt)) DMA_IRQHandler()
{
  *DMA_IFC = 1;
  *DMA_CHENS = 1;
}

/* Enabled with DMA */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;
}

/* Plays the tone set by global value global_freq when a switch is pressed
 * Button press enables timer interrupt, button release disables */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
  *LETIMER0_IFC = 1;
  if (STARTUPSONG) {
    playStartUpSong();
  } else {
    playTone(global_freq);
  }
}

/* GPIO even pin interrupt handler. Plays different tones for
 * every swtich. Switch 7 plays tone using DMA. Notice setups
 * and disables relative to switch push-position (in / out) */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
  *GPIO_IFC = 0xff;

  if (*GPIO_PC_DIN != 0xff)
  {
    setupDAC(0);
    setupLowEnergyTimer();
    switch(*GPIO_PC_DIN)
    {
      case SWITCH_1:
        global_freq = A;
        setLed(1);
        break;
      case SWITCH_3:
        global_freq = B;
        setLed(3);
        break;
      case SWITCH_5:
        global_freq = C;
        setLed(5);
        break;
      case SWITCH_7:
        *SCR = 2;
        disableLowEnergyTimer();
        setupDAC(1);
        setLed(7);
        setupTimer(SAMPLE_FREQ);
        setupPRS();
        setupDMA();
        break;
    }
  } else {
    disableDMA();
    disablePRS();
    disableTimer();
    disableLowEnergyTimer();
    disableDAC();
    *GPIO_PA_DOUT = 0xffff;
    *SCR = 6;
  }
}

/* GPIO odd pin interrupt handler. Plays different tones for
 * every swtich. */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
  *GPIO_IFC = 0xff;

  if (*GPIO_PC_DIN != 0xff) {
    setupDAC(0);
    setupLowEnergyTimer();
    switch(*GPIO_PC_DIN) {
      case SWITCH_2:
        global_freq = E;
        setLed(2);
        break;
      case SWITCH_4:
        global_freq = F;
        setLed(4);
        break;
      case SWITCH_6:
        global_freq = G;
        setLed(6);
        break;
      case SWITCH_8:
        STARTUPSONG = 1;
        setLed(8);
        break;
    }
  } else {
    STARTUPSONG = 0;
    disableLowEnergyTimer();
    disableDAC();
    *GPIO_PA_DOUT = 0xffff;
  }
}
