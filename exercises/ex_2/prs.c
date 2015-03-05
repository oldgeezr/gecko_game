#include "efm32gg.h"
#include "prs.h"

void setupPRS(void)
{
  *CMU_HFPERCLKEN0 |= HFPER_PRS;
  *PRS_CH0_CTRL = (0x34 << 16); //source LETIMER0
}
