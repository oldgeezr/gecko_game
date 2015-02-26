#include <stdbool.h>

#define SINGLE_ENDED (0 << 1)
#define DIFFERENTIAL (1 << 1)
#define OUTMODE_PIN (1 << 5)
#define PRESC_1 (1 << 16)
#define PRESC_2 (1 << 17)
#define PRESC_3 (1 << 18)

#define HFPER_DAC0 (1 << 17)
#define HFPER_PRS (1 << 15)

typedef struct
{
	uint32_t src_data_end_ptr;
	uint32_t dst_data_end_ptr;
	uint32_t channel_cfg;
} configDMA_t;

void setupDAC(void)
{
   *CMU_HFPERCLKEN0 |= HFPER_DAC0;
   *DAC0_CTRL = DIFFERENTIAL | OUTMODE_PIN | PRESC_1 | PRESC_3;
   *DAC0_CH0CTRL = true;
   *DAC0_CH1CTRL = true;
}

void disableDAC(void) 
{
	*CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
}

void setupDMA(void) 
{
	*DMA_CH0_CTRL = (1 << 19) | (1 << 17); //DAC0

	configDMA_t config;
	config.src_data_end_ptr = 0;
	config.dst_data_end_ptr = 0;
	config.channel_cfg = 0xAA000000;

	*DMA_CTRLBASE = 0;

	*DMA_CONFIG = true;
	*DMA_CHUSEBURSTS = true;
	*DMA_REQMASKC = true;
	*DMA_CHALTC = true;
	*DMA_CHENS = true;
}

void setupPRS(void)
{
	*CMU_HFPERCLKEN0 |= HFPER_PRS;
	*PRS_CH0_CTRL = 0;
}
