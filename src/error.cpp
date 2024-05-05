#include "error.hpp"

#include <libGUI.hpp>
#include <libSerial.hpp>
#include <system.hpp>

#ifdef TEXT_MODE
using namespace TextRenderer;
#else
using namespace GuiRenderer;
#endif

void error(string msg, bool fatal) {
    serialWriteStr("ERROR: ");
    serialWriteStr(msg);
    serialWriteStr("\r\n");


    setDrawColor(VGA_RED);
    print("ERROR: ");
    print(msg);
    println("");

    if (fatal) {
        serialWriteStr("FATAL: System Halting!\r\n");
        print("FATAL: System Halting!");
        disableInterrupts();
        halt();
    }
}