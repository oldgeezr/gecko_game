#ifndef _DMA_H_
#define _DMA_H_

#include "efm32gg.h"

#define DMA_BUFFER_SIZE       40

#define BYTE                  0x00
#define HALFWORD              0x01
#define WORD                  0x02
#define NO_INCREMENT          0x03

#define CYCLE_CTRL_BASIC      0x01
#define CYCLE_CTRL_AUTO_REQ   0x02
#define CYCLE_CTRL_PING_PONG  0x03

static uint16_t dma_buffer[DMA_BUFFER_SIZE] = {
	0, 50, 100, 150, 200, 250, 300, 350, 400, 450,
	500, 550, 600, 650, 700, 750, 800, 850, 900, 950,
	900, 850, 800, 750, 700, 650, 600, 550, 500, 450,
	400, 350, 300, 250, 200, 150, 100, 50, 0
};

typedef struct
{
	uint32_t *src_data_end_ptr;
	uint32_t *dst_data_end_ptr;
	uint32_t channel_cfg;
} configDMA_t;

typedef struct
{
	uint16_t dst_inc;
	uint16_t dst_size;
	uint16_t src_inc;
	uint16_t src_size;
	uint16_t dst_prot_ctrl;
	uint16_t src_prot_ctrl;
	uint16_t r_power;
	uint16_t n_minus_1;
	uint16_t cycle_ctrl;
} controlData_t;

void setupDMA(void);

#endif
