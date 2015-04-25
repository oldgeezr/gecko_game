/*
 * game.c
 *
 *  Created on: 17. apr. 2015
 *      Author: jonzk_000
 */

#include "graphics.h"
#include "display.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Color palette
color_t blue   = {31,0,0},
				red    = {0,0,31},
				green  = {0,63,0},
				yellow = {0,63,31},
				cyan   = {31,63,0},
				pink   = {31,0,31},
			  white  = {31,63,31},
				gray   = {50,50,50},
				black  = {0,0,0};

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void GRAPHICS_printBall(uint16_t x, uint16_t y) {

	display_draw_filled_rect(x,y,BALL_RADIUS,BALL_RADIUS,red);
	display_refresh(x,y,BALL_RADIUS,BALL_RADIUS);
}

void GRAPHICS_clearBall(uint16_t x, uint16_t y) {

	display_draw_filled_rect(x,y,BALL_RADIUS,BALL_RADIUS,black);
	display_refresh(x,y,BALL_RADIUS,BALL_RADIUS);
}

void GRAPHICS_printBar(uint16_t x) {

	display_draw_filled_rect(x,HEIGHT-BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT,white);
	display_refresh(x,HEIGHT-BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT);
}

void GRAPHICS_clearBar(uint16_t x) {

	display_draw_filled_rect(x,HEIGHT-BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT,black);
	display_refresh(x,HEIGHT-BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT);
}

void GRAPHICS_showWelcomeScreen(void) {

	char *str = "Press a button to continue";
	char *str2 = "By: Christoffer R-e, Erlend H. and Jon Z.K.";

	GRAPHICS_clearScreen();
	display_puts(WIDTH/2-3*strlen(str2),20,str2,white,1);
	display_draw_filled_rect(0,70,WIDTH,5,green);
	display_puts(50, 110, "Game Of Pong",white,3);
	display_draw_filled_rect(0,160,WIDTH,5,green);
	display_puts(WIDTH/2 - 3*strlen(str),200,str,white,1);
	display_refresh(0,0,WIDTH,HEIGHT);
}

void GRAPHICS_gameOver(void) {

	display_puts(80,120,"Game Over",white,3);
	display_refresh(0,0,WIDTH,HEIGHT);
}

void GRAPHICS_printScore(uint16_t score) {

	char str[10];

	display_puts(5,5,"Score:",white,1);

	if(score > 0) {
		sprintf(str, "%d", score-1);
		display_puts(40,5,str,black,1);
	}

	sprintf(str, "%d", score);
	display_puts(40,5,str,white,1);
	display_refresh(0,0,60,20);
}

void GRAPHICS_printBackground(void) {

	display_draw_line(0,20,WIDTH,20,white);
	display_refresh(0,0,WIDTH,HEIGHT);
}

void GRAPHICS_printLevel(uint16_t level) {

	char str[10];

	display_puts(60,5,"Level:",white,1);

	if(level > 0) {
		sprintf(str, "%d", level-1);
		display_puts(95,5,str,black,1);
	}

	sprintf(str, "%d", level);
	display_puts(95,5,str,white,1);
	display_refresh(60,0,60,20);

}

void GRAPHICS_clearScreen(void) {

	display_draw_filled_rect(0,0,WIDTH,HEIGHT,black);
	display_refresh(0,0,WIDTH,HEIGHT);
}
