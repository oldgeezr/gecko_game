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

void GRAPHICS_printBall(uint16_t x, uint16_t y) {
	display_draw_filled_circle(x,y,BALL_RADIUS,cyan);
}

void GRAPHICS_clearBall(uint16_t x, uint16_t y) {
	display_draw_filled_circle(x,y,BALL_RADIUS,black);
}

void GRAPHICS_showWelcomeScreen(void) {
	GRAPHICS_clearScreen();
	display_puts(50, 10, "Game Of Zwag",white,3);
	display_puts(100,200,"Press a button to continue",white,1);
	display_draw_filled_circle(WIDTH/2, 100, 25, pink);
	display_draw_filled_circle(WIDTH/2, 150, 25, white);
	display_refresh();
}

void GRAPHICS_clearScreen(void) {
	display_draw_filled_rect(0,0,WIDTH,HEIGHT,black);
}
void GRAPHICS_printBar(uint16_t x) {
	display_draw_filled_rect(x,HEIGHT-BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT,yellow);
}
void GRAPHICS_clearBar(uint16_t x) {
	display_draw_filled_rect(x,HEIGHT-BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT,black);
}


