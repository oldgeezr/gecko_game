#include "efm32gg.h"
#include "prs.h"

#define HFPER_PRS (1 << 15)

void setupPRS(void)
{
  *CMU_HFPERCLKEN0 |= HFPER_PRS;
  *PRS_CH0_CTRL = (0x1d << 16) | (0x1 << 0);
}

void disablePRS(void)
{
  *PRS_CH0_CTRL = 0;
  *CMU_HFPERCLKEN0 &= ~HFPER_PRS;
}

