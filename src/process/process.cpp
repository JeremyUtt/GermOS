
#include <process.hpp>
#include <scheduler.hpp>
Process::Process(string name, uint32_t entrypoint, UiMode uiMode) {
    // _cState;
    _name = name;
    _pState = Initalized;
    _entrypoint = entrypoint;
    _uiMode = uiMode;

    _cState.eax = 0;
    _cState.ecx = 0;
    _cState.edx = 0;
    _cState.ebx = 0;

    _cState.esp = 0;
    _cState.ebp = 0;

    _cState.esi = 0;
    _cState.edi = 0;

    _cState.eip = 0;
    _cState.eflags = 0;

    _cState.cs = 0x8;
    _cState.ss = 0;
    _cState.ds = 0;
    _cState.es = 0;
    _cState.fs = 0;
    _cState.gs = 0;
}

void Process::start() {
    bool needToChangeMode = false;
    if (getUiMode() != _uiMode) {
        setUiMode(_uiMode);
        needToChangeMode = true;
    }

    _pState = Running;

    auto func = (void (*)())_entrypoint;
    func();
    _pState = Exited;

    if (needToChangeMode) {
        if (_uiMode == GRAPHICS) {
            setUiMode(TEXT);
        } else {
            setUiMode(GRAPHICS);
        }
    }
}

void Process::kill() {
    _pState = Exited;
}

void Process::pause() {
    // Not properly implemented yet
    return;

    _pState = Paused;
}

void Process::resume() {
    // Not properly implemented yet
    return;

    _pState = Running;
}
