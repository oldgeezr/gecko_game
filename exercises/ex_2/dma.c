#include <stdint.h>
#include <stdbool.h>
#include "dma.h"
#include "efm32gg.h"

void setupDMA(void)
{
  *DMA_CH0_CTRL = (0x0A << 16); //DAC0
  *DMA_CH0_LOOP = (1 << 16); //Enable looping

  configDMA_t config;
  config.src_data_end_ptr = (uint32_t *)(dma_buffer + DMA_BUFFER_SIZE);
  config.dst_data_end_ptr = ((uint32_t *)DAC0_COMBDATA);

  controlData_t control;
  control.dst_inc = NO_INCREMENT;
  control.dst_size = WORD;
  control.src_inc = WORD;
  control.src_size = WORD;
  control.n_minus_1 = (DMA_BUFFER_SIZE - 1);
  control.cycle_ctrl = CYCLE_CTRL_BASIC;

  config.channel_cfg = (control.dst_inc << 30) | (control.dst_size << 28) | (control.src_inc << 26)
             | (control.src_size << 24) | (control.n_minus_1 << 4) | (control.cycle_ctrl << 0);

  //SrcEngPtr
  *(DMA_CTRLBASE + 0x000) = *(config.src_data_end_ptr);
  //DstEndPtr
  *(DMA_CTRLBASE + 0x004) = *(config.dst_data_end_ptr);
  //ch_cfg
  *(DMA_CTRLBASE + 0x008) = config.channel_cfg;

  *DMA_CONFIG = true;
  *DMA_CHUSEBURSTS = true;
  *DMA_REQMASKC = true;
  *DMA_CHALTC = true;
  *DMA_CHENS = true;
}

// SHOULD HAVE A DMA DISABLE FUNCTION
