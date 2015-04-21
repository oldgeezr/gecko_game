#include "display.h"
#include "font.h"

struct fb_var_screeninfo display_info;

//-----------------------Geometric functions----------------------------------//

void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color) {

	uint16_t steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
	ystep = -1;}

	for (; x0<=x1; x0++) {
		if (steep) {
			display_draw_pixel(y0, x0, color);
			} else {
			display_draw_pixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void display_draw_filled_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color) {

	uint16_t i,j;

	for(i = x; i < x + width; i++) {
		for(j = y; j < y + height; j++) {
			display_draw_pixel(i,j,color);
		}
	}
}

void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, color_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	display_draw_pixel(x0, y0+r, color);
	display_draw_pixel(x0, y0-r, color);
	display_draw_pixel(x0+r, y0, color);
	display_draw_pixel(x0-r, y0, color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		display_draw_pixel(x0 + x, y0 + y, color);
		display_draw_pixel(x0 - x, y0 + y, color);
		display_draw_pixel(x0 + x, y0 - y, color);
		display_draw_pixel(x0 - x, y0 - y, color);

		display_draw_pixel(x0 + y, y0 + x, color);
		display_draw_pixel(x0 - y, y0 + x, color);
		display_draw_pixel(x0 + y, y0 - x, color);
		display_draw_pixel(x0 - y, y0 - x, color);
	}
}

void display_draw_filled_circle(uint16_t x0, uint16_t y0, uint16_t r, color_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	display_draw_line(x0, y0-r, x0, y0+r+1, color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		display_draw_line(x0+x, y0-y, x0+x, y0+y+1, color);
		display_draw_line(x0-x, y0-y, x0-x, y0+y+1, color);
		display_draw_line(x0+y, y0-x, x0+y, y0+x+1, color);
		display_draw_line(x0-y, y0-x, x0-y, y0+x+1, color);
	}
}

//-------------------------Char functions------------------------------------------//

void display_draw_char(uint16_t x, uint16_t y, char c, color_t color, uint8_t size) {

	uint8_t i,j;

	for (i = 0; i < 5; i++) {
	  uint8_t line = font[(c*5)+i];
	  for (j = 0; j < 8; j++) {
		  if (line & 0x1) {
			  if (size == 1) // default size
					display_draw_pixel(x+i, y+j, color);
			  else {  // big size
				  display_draw_filled_rect(x+i*size, y+j*size, size, size, color);
			  }
		  }
		  line >>= 1;
	  }
  }
}

void display_puts(uint16_t x, uint16_t y, char *str, color_t color, uint8_t size) {

	uint16_t i = 0;

	while(*str) {
		display_draw_char(x+i*(6*size),y,*str++,color,size);
		i++;
	}
}

//------------------------Setup---------------------------------------------------//

display_status_t display_setup(void) {

	rect.dx = 0;
	rect.dy = 0;
	rect.width = WIDTH;
	rect.height = HEIGHT;

	fb = open("/dev/fb0",O_RDWR);

	if(!fb) {
		puts("Fail: Could not open framebuffer...");
		exit(EXIT_FAILURE);
	}

	if(ioctl(fb,FBIOGET_VSCREENINFO,&display_info) == -1) {
		puts("Fail: Could not get screen info...");
		return ERROR;
	}

	uint16_t size = (WIDTH * HEIGHT) * display_info.bits_per_pixel/8;
	display_buffer = (color_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,fb,0);

	return SUCCESS;
}
