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
#define BAR_LENGTH		40		// Pixels
#define BAR_HEIGHT		6			// Pixels

#define BALL_RADIUS		6			// Pixels
#define BALL_DIAMETER (BALL_RADIUS * 2)

void GRAPHICS_showWelcomeScreen(void);
void GRAPHICS_clearScreen(void);
void GRAPHICS_printBall(uint16_t x, uint16_t y);
void GRAPHICS_printBar(uint16_t x);
void GRAPHICS_clearBall(uint16_t x, uint16_t y);
void GRAPHICS_clearBar(uint16_t x);
void GRAPHICS_printScore(uint16_t score);
void GRAPHICS_printLevel(uint16_t level);
void GRAPHICS_printBackground(void);
void GRAPHICS_gameOver(void);

#endif /* GRAPHICS_H_ */
