#pragma once
#include <stdint.h>
#include <stddef.h>

#include <libVGA.hpp>
#include <string.hpp>


enum ProcessState { Initalized, Running, Queued, Paused, Exited };

struct CpuState {
    int eax;
    int ebx;
    int ecx;
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


static_assert(sizeof(CpuState) == 64, "CpuState layout must remain 64 bytes");
static_assert(offsetof(CpuState, esp) == 16, "CpuState::esp offset changed");
static_assert(offsetof(CpuState, eip) == 32, "CpuState::eip offset changed");
static_assert(offsetof(CpuState, fs) == 56, "CpuState::fs offset changed");
static_assert(offsetof(CpuState, gs) == 60, "CpuState::gs offset changed");

extern "C" void storeState(CpuState* address, uint32_t* interrupt_frame,
                            uint32_t* register_snapshot);
extern "C" uint8_t createFrame();
extern "C" void asmfunction();


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
