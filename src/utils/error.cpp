#include "error.hpp"

#include <libGUI.hpp>
#include <libSerial.hpp>
#include <printf.hpp>
#include <system.hpp>

void error(string msg, bool fatal) {
    serialWriteStr("ERROR: ");
    serialWriteStr(msg);
    serialWriteStr("\n");

    // ren.setDrawColor(RED);
    printf("ERROR: ");
    printf(msg);
    printf("\n");

    if (fatal) {
        serialWriteStr("FATAL: System Halting!\r\n");
        printf("FATAL: System Halting!");
        disableInterrupts();
        halt();
    }
}