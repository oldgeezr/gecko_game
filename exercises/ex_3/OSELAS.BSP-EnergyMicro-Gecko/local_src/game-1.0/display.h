#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 320
#define HEIGHT 240

#define swap(a, b) { uint16_t t = a; a = b; b = t; }

typedef struct {
	uint16_t r : 5;
	uint16_t g : 6;
	uint16_t b : 5;
} color_t;

typedef enum {
	SUCCESS,
	ERROR,
	FAILURE
} display_status_t;

color_t *display_buffer;

struct fb_copyarea rect;
int fb;

//-----------------------Small functions--------------------------------------//

static inline void display_draw_pixel(uint16_t x, uint16_t y, color_t color) {
	display_buffer[x + y * WIDTH] = color;
}

static inline void display_clear_pixel(uint16_t x, uint16_t y) {
	//display_buffer[x + y * WIDTH] = {0,0,0};
}

static inline void display_refresh(uint16_t dx, uint16_t dy, uint16_t w, uint16_t h) {
	rect.dx = dx;
	rect.dy = dy;
	rect.width = w;
	rect.height = h;
	ioctl(fb, 0x4680, &rect);
}

//----------------------Geometric functions-----------------------------------//

void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);
void display_draw_filled_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color);
void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, color_t color);
void display_draw_filled_circle(uint16_t x0, uint16_t y0, uint16_t r, color_t color);

//---------------------Char functions-----------------------------------------//
void display_draw_char(uint16_t x, uint16_t y, char c, color_t color, uint8_t size);
void display_puts(uint16_t x, uint16_t y, char *str, color_t color, uint8_t size);

display_status_t display_setup(void);

#endif
