/*
 * game.c
 *
 *  Created on: 17. apr. 2015
 *      Author: jonzk_000
 */

#include "graphics.h"
#include "display.h"

color_t blue   = {31,0,0},
        red    = {0,0,31},
        green  = {0,63,0},
        yellow = {0,63,31},
        cyan   = {31,63,0},
        pink   = {31,0,31},
        white  = {31,63,31},
        black  = {0,0,0};

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void GRAPHICS_printBall(uint16_t x, uint16_t y)
{
	uint16_t a,b;

	for(a = x - BALL_RADIUS; a <= x + BALL_RADIUS; a++)
	{
		for(b = y - BALL_RADIUS; b <= y + BALL_RADIUS; b++)
		{
			if ( ((a*a) + (b*b)) < (BALL_RADIUS*BALL_RADIUS) )
			{
				display_draw_pixel(a,b, white);
			}
		}
	}
  display_refresh();
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
				display_draw_pixel(a,b, black);
			}
		}
	}
}

void GRAPHICS_showWelcomeScreen(void) {}
void GRAPHICS_clearScreen(void) {}
void GRAPHICS_printBar(uint16_t x) {}
