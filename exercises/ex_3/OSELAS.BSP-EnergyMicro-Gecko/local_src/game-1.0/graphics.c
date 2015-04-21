/*
 * game.c
 *
 *  Created on: 17. apr. 2015
 *      Author: jonzk_000
 */

#include <stdint.h>

#include "graphics.h"


void printPixel(uint16_t x, uint16_t y, uint16_t color);

void GRAPHICS_printBall(uint16_t x, uint16_t y)
{
	uint16_t a,b;

	for(a = x - BALL_RADIUS; a <= x + BALL_RADIUS; a++)
	{
		for(b = y - BALL_RADIUS; b <= y + BALL_RADIUS; b++)
		{
			if ( ((a*a) + (b*b)) < (BALL_RADIUS*BALL_RADIUS) )
			{
				printPixel(a,b, BALL_COLOR);
			}
		}
	}
}

void GRAPHICS_clearBall(uint16_t x, uint16_t y)
{
	uint16_t a,b;

	for(a = x - BALL_RADIUS; a <= x + BALL_RADIUS; a++)
	{
		for(b = y - BALL_RADIUS; b <= y + BALL_RADIUS; b++)
		{
			if ( ((a*a) + (b*b)) < (BALL_RADIUS*BALL_RADIUS) )
			{
				printPixel(a,b, BCKGRND_COLOR);
			}
		}
	}
}

void GRAPHICS_printBar(uint16_t x)
{
	uint16_t a,b;
	if()
}



void printPixel(uint16_t x, uint16_t y, uint16_t color)
{
	/* Brukes fra API */
}
