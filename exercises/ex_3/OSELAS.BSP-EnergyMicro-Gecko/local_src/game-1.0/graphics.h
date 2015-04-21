/*
 * graphics.h
 *
 *  Created on: 19. apr. 2015
 *      Author: jonzk_000
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>

/* Graphic attributes */
#define BAR_LENGTH  	40 		// Pixels
#define BAR_HEIGHT		5 		// Pixels
#define BAR_COLOR   	0xFFF 	// RGB hexadecimals
#define BALL_RADIUS 	5 		// Pixels
#define BALL_COLOR  	0xFFF 	// RGB hexadecimals
#define BCKGRND_COLOR 	0x000 	// RGB hexadecimals

void GRAPHICS_showWelcomeScreen(void);
void GRAPHICS_clearScreen(void);
void GRAPHICS_printBall(uint16_t x, uint16_t y);
void GRAPHICS_printBar(uint16_t x);
void GRAPHICS_clearBall(uint16_t x, uint16_t y);
//void GRAPHICS_clearBar(uint16_t x);

#endif /* GRAPHICS_H_ */
