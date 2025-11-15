#pragma once

// Wait for any interrupt to occur
#define halt() asm("hlt");

#define disableInterrupts() asm("cli");