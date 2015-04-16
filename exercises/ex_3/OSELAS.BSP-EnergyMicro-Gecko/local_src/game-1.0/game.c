#include <stdio.h>
#include <stdlib.h>

#include "display.h"

color_t blue	 = {31,0,0},
				red    = {0,0,31},
				green  = {0,63,0},
				yellow = {0,63,31},
				cyan   = {31,63,0},
				pink	 = {31,0,31},
				white  = {31,63,31},
				black  = {0,0,0};

int main(int argc, char *argv[])
{
	puts("Display test:");
	display_setup();
	puts("Display setup success!");
	display_puts(10,100,"Lol!",red,3);
	display_refresh();
	//display_fill_screen(0x00FF);
	puts("Draw rect success!");
	exit(EXIT_SUCCESS);
}
