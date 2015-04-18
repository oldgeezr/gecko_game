//`AC_DEFINE(_GNU_SOURCE)'
//     GNU packages should normally include this line before any other
//     feature tests.  This defines the macro `_GNU_SOURCE' when
//     compiling, which directs the libc header files to provide the
//     standard GNU system interfaces including all GNU extensions.  If
//     this macro is not defined, certain GNU extensions may not be
//     available.

#ifndef _POSIX_C_SOURCE
//#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

//#include "display.h"
//
//color_t blue	 = {31,0,0},
//				red    = {0,0,31},
//				green  = {0,63,0},
//				yellow = {0,63,31},
//				cyan   = {31,63,0},
//				pink	 = {31,0,31},
//				white  = {31,63,31},
//				black  = {0,0,0};

int descr;
int buff;

void interrupt_handler(int signo) {
	//if(signum ==SIGIO){
	read((descr), &buff, 1);
	printf("type: %d reading %d\n", signo, buff);
	//}
}

int main(int argc, char *argv[])
{

	int oflags;
	descr = open("/dev/gamepad", O_RDONLY);
	signal(SIGIO, &interrupt_handler);
	fcntl(descr, F_SETOWN, getpid());
	oflags = fcntl(descr, F_GETFL);
	fcntl(descr, F_SETFL, oflags | FASYNC);
	printf("Started game\n");

	for (;;) {
		sleep(10);
	}

	return 0;
}

