#include <stdbool.h>

#define SINGLE_ENDED (0 << 1)
#define DIFFERENTIAL (1 << 1)
#define OUTMODE_PIN (1 << 5)
#define PRESC_1 (1 << 16)
#define PRESC_2 (1 << 17)
#define PRESC_3 (1 << 18)

#define HFPER_DAC0 (1 << 17)
#define HFPER_PRS (1 << 15)

#define DMA_BUFFER_SIZE 32 
static uint16_t dma_buffer[DMA_BUFFER_SIZE]

typedef struct
{
	uint32_t *src_data_end_ptr;
	uint32_t *dst_data_end_ptr;
	uint32_t channel_cfg;
} configDMA_t;

void setupDAC(void)
{
   *CMU_HFPERCLKEN0 |= HFPER_DAC0;
   *DAC0_CTRL = DIFFERENTIAL | OUTMODE_PIN | PRESC_1 | PRESC_3;
   //Trigger DAC from PRS0 input
   *DAC0_CH0CTRL = (1 << 2) | (1 << 1);
   *DAC0_CH1CTRL = (1 << 2) | (1 << 1);
}

void disableDAC(void) 
{
	*CMU_HFPERCLKEN0 &= ~(HFPER_DAC0);
}

void setupDMA(void) 
{
	*DMA_CH0_CTRL = (1 << 19) | (1 << 17); //DAC0
	//DMA_BUFFER_SIZE-1

	configDMA_t config;
	config.src_data_end_ptr = dma_buffer + DMA_BUFFER_SIZE;
	config.dst_data_end_ptr = DAC0_COMBDATA;
	//src inc = word
	//src size = word
	//dst inc = no increment
	//dst size = word
	//cycle 

	config.channel_cfg = 0xA2000001 | (DMA_BUFFER_SIZE << 4) ;

	//SrcEngPtr
	*(DMA_CTRLBASE + 0x000) = 0;
	//DstEndPtr
	*(DMA_CTRLBASE + 0x004) = 0;
	//ch_cfg
	*(DMA_CTRLBASE + 0x008) = 0;

	*DMA_CONFIG = true;
	*DMA_CHUSEBURSTS = true;
	*DMA_REQMASKC = true;
	*DMA_CHALTC = true;
	*DMA_CHENS = true;
}

void setupPRS(void)
{
	*CMU_HFPERCLKEN0 |= HFPER_PRS;
	// Select LETIMER0
	*PRS_CH0_CTRL = (1 << 21) | (1 << 20) | (1 << 18);
}
