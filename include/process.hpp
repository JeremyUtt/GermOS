#pragma once
#include <stdint.h>

#include <libVGA.hpp>
#include <string.hpp>
enum ProcessState { Initalized, Running, Queued, Paused, Exited };

struct CpuState {
    int eax;
    int ecx;
    int ebx;
    int edx;

    int esp;  // Stack Pointer
    int ebp;  // Stack Base Pointer

    int esi;  // Source Index register      (??)
    int edi;  // Destination Index register (??)

    int eip;  // Program Counter
    int eflags;

    int cs;  // Code segment selector
    int ss;  // Stack segment selector
    int ds;  // Data segment selector
    int es;  // Extra segment selector
    int fs;  // Extra segment selector
    int gs;  // Extra segment selector
};

extern "C" uint8_t storeState(CpuState* address);

class Process {
  private:
    ProcessState _pState;
    CpuState _cState;
    string _name;
    uint32_t _entrypoint;
    static CpuState _dispatcherState;
    UiMode _uiMode;

  public:
    Process(string name, uint32_t entrypoint, UiMode uiMode);
    void start();
    void kill();
    // void saveManagerState();

    // Not properly implemented yet
    void pause();

    // Not properly implemented yet
    void resume();
};
