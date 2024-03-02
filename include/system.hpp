#pragma once

#define halt() asm("hlt");
#define disableInterrupts() asm("cli");