#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

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

void interrupt_handler(int n, siginfo_t *info, void* arg);

int main(int argc, char *argv[])
{

	int gamepad;

	gamepad = open("/dev/gamepad", O_RDWR);
	if (gamepad < 0) {
		printf("Could not open Gamepad Driver...");
	}

	struct sigaction gamepad_signal;

	gamepad_signal.sa_sigaction = interrupt_handler;
	gamepad_signal.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &gamepad_signal, NULL);

	for (;;) {
		sleep(1000);
	};

	exit(EXIT_SUCCESS);
}

void interrupt_handler(int n, siginfo_t *info, void* arg) {

	uint8_t buttons = (uint8_t)(info->si_int);
	printf("Button registers: %x\n", buttons);
}
