#pragma once
#include <stddef.h>
#include <stdint.h>

#include <libVGA.hpp>
#include <string.hpp>

enum ProcessState { Initalized, Running, Queued, Paused, Exited };

struct CpuState {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esp;  // Stack Pointer
    uint32_t ebp;  // Stack Base Pointer

    uint32_t esi;  // Source Index register      (??)
    uint32_t edi;  // Destination Index register (??)

    uint32_t eip;  // Program Counter
    uint32_t eflags;

    uint32_t cs;  // Code segment selector
    uint32_t ss;  // Stack segment selector
    uint32_t ds;  // Data segment selector
    uint32_t es;  // Extra segment selector
    uint32_t fs;  // Extra segment selector
    uint32_t gs;  // Extra segment selector
};
static_assert(sizeof(CpuState) == 64, "CpuState layout must remain 64 bytes");
static_assert(offsetof(CpuState, esp) == 16, "CpuState::esp offset changed");
static_assert(offsetof(CpuState, eip) == 32, "CpuState::eip offset changed");
static_assert(offsetof(CpuState, fs) == 56, "CpuState::fs offset changed");
static_assert(offsetof(CpuState, gs) == 60, "CpuState::gs offset changed");

// the values automatically pushed to the stack when a ISR is called
struct InterruptFrame {
    uint32_t ip;
    uint32_t cs;
    uint32_t eflags;
};
static_assert(sizeof(InterruptFrame) == 12, "InterruptFrame layout must remain 12 bytes");
static_assert(offsetof(InterruptFrame, ip) == 0, "InterruptFrame::ip offset changed");
static_assert(offsetof(InterruptFrame, eflags) == 8, "CpuState::esp offset changed");

// the values pushed to the stack bt the custom Timer Handler function
struct RegisterSnapshot {
    uint32_t edi;
    uint32_t esi;
    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    uint32_t ebp;
    uint32_t esp;
};
static_assert(sizeof(RegisterSnapshot) == 32, "RegisterSnapshot layout must remain 32 bytes");


// extern "C" void storeState(CpuState* address, uint32_t* interrupt_frame, uint32_t* register_snapshot);
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
