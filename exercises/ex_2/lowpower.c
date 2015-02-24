#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

static inline void enableDeepsleep(void) {
	*SCR = 6;
}

static inline void disableRamBlocks(void) {
	*EMU_MEMCTRL = 7;
}

void setupLowpower(void) {
	enableDeepsleep();
	disableRamBlocks();
}