#pragma once
#include <stdint.h>

#include <libVGA.hpp>
#include <string.hpp>
#include <scheduler.hpp>
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
