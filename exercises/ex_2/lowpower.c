#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

static inline void enableDeepSleep(void) {
	*SCR = 6;
}

static inline void disableRamBlocks(void) {
	*EMU_MEMCTRL = 7;
}

void setupLowPowerMode(void) {
	enableDeepsleep();
	disableRamBlocks();
}
