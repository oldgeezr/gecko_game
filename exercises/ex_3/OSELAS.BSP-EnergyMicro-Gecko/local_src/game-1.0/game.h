/*
 * game.h
 *
 *  Created on: 19. apr. 2015
 *      Author: jonzk_000
 */

#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#define DISPLAY_DEVICE_NO	(0)

#include "graphics.h"

/* Masks */
#define LEFT				-1
#define RIGHT				1
#define UP					-1
#define DOWN				1
#define BAR_MOVE		10
#define BALL_MOVE   6

/* System constants */
#define SCREEN_RES_Y	240		// Pixels
#define SCREEN_RES_X	320		// Pixels
#define FPS				1
#define X_MAX     20

#endif /* GAME_H_ */
