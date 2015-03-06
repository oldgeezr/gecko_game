#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "efm32gg.h"

#define DIFFERENTIAL (1 << 0)
#define OUTMODE_PIN  (0x1 << 4)
#define PRESC 		 (0x5 << 16)

#define HFPER_DAC0 (1 << 17)
#define HFPER_PRS (1 << 15)

#define DMA_BUFFER_SIZE 8000 

#define BYTE 0x00
#define HALFWORD 0x01
#define WORD 0x02
#define NO_INCREMENT 0x03

#define CYCLE_CTRL_BASIC 0x01
#define CYCLE_CTRL_AUTO_REQ 0x02
#define CYCLE_CTRL_PING_PONG 0x03

#define SAMPLING_FREQ 32768

#define PI (3.141592653589793)

#define DMACTRL_CH_CNT      16
#define DMACTRL_ALIGNMENT   512

#define A 440
#define B 494
#define Bb 466
#define C 261
#define D 294
#define E 330
#define Eb 311
#define F 350
#define G 392


static uint16_t dma_buffer[DMA_BUFFER_SIZE] = {
	0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 
	500, 550, 600, 650, 700, 750, 800, 850, 900, 950,
	900, 850, 800, 750, 700, 650, 600, 550, 500, 450,
	400, 350, 300, 250, 200, 150, 100, 50, 0 
};


//uint16_t dma_buffer[DMA_BUFFER_SIZE];

typedef struct
{
	volatile void * volatile src_data_end_ptr;
	volatile void * volatile dst_data_end_ptr;
	volatile uint32_t channel_cfg;
	volatile uint32_t user;
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

configDMA_t dmaControlBlock[DMACTRL_CH_CNT * 2] __attribute__ ((aligned(DMACTRL_ALIGNMENT)));

void setupDAC(void);
void disableDAC(void);
void setupDMA(void);
void setupPRS(void);

static inline void writeDAC(uint16_t data) 
{
	*DAC0_COMBDATA = data;
}

static inline void playSine(void) 
{
	static uint16_t index = 0;
	writeDAC(dma_buffer[index++]);
	if (index == DMA_BUFFER_SIZE)
	{
		index = 0;
	}
}

static inline uint16_t dacOutputLevel(uint16_t amplitude, double angle, double N) {
	return (amplitude/2 + amplitude*sin(((2*PI)/N)*angle));
}

static inline void playTone(uint16_t tone_freq) 
{
	static uint16_t i = 0;
	uint16_t N = SAMPLING_FREQ/(tone_freq*10);
	*DAC0_COMBDATA = dacOutputLevel(1000,i,N);
	i = i % N;
	i++;
}

static inline void playSquare(uint16_t tone) 
{
	uint16_t N = (2*SAMPLING_FREQ)/tone;

	static uint16_t i = 0;

	if (i < N/2)
	{
		writeDAC(500);
	}
	else {
		writeDAC(0);
	}
	i %= N;
	i++;
}

#endif