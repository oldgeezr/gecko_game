#include <stdint.h>
#include "dma.h"
#include "efm32gg.h"

void setupDMA(void)
{
  *CMU_HFCORECLKEN0 |= (1 << 0);
  *DMA_CTRLBASE = (uint32_t)(dmaControlBlock);
  *DMA_CONFIG = 1;

  *DMA_CH0_CTRL = (0x0A << 16);
  *DMA_CH0_LOOP |= (1 << 16);
  *DMA_CH0_LOOP |= (DMA_BUFFER_SIZE - 1);

  configDMA_t *primDescr;

  primDescr = (configDMA_t *)(*DMA_CTRLBASE);

  controlData_t control;

  control.dst_inc = NO_INCREMENT;
  control.dst_size = HALFWORD;
  control.src_inc = HALFWORD;
  control.src_size = HALFWORD;
  control.n_minus_1 = (DMA_BUFFER_SIZE - 1);
  control.cycle_ctrl = CYCLE_CTRL_BASIC;

  primDescr->src_data_end_ptr = (void *)((uint32_t)dma_buffer + (control.n_minus_1 << 1));
  primDescr->dst_data_end_ptr = (void *)((uint32_t)DAC0_COMBDATA);
  primDescr->channel_cfg = (control.dst_inc << 30) | (control.dst_size << 28) | (control.src_inc << 26)
    | (control.src_size << 24) | (control.n_minus_1 << 4) | (control.cycle_ctrl << 0);

  *DMA_CHALTC = 1;
  *DMA_CHENS = 1;
}

void disableDMA(void)
{
  *DMA_CTRLBASE = 0;
  *DMA_CONFIG = 0;
  *DMA_CH0_CTRL = 0;
  *DMA_CH0_LOOP = 0;
  *DMA_CHALTC = 0;
  *DMA_CHENS = 0;
  *CMU_HFCORECLKEN0 &= ~(1 << 0);
}
