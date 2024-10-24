
#include <process.hpp>

Process::Process(string name, uint32_t entrypoint, UiMode uiMode) {
    _cState = {0};
    _name = name;
    _pState = Initalized;
    _entrypoint = entrypoint;
    _uiMode = uiMode;

    _cState.eax = 0;
    _cState.ebx = 0;
    _cState.ecx = 0;
    _cState.edx = 0;

    // _cState.esi;  // TODO
    // _cState.edi;  // TODO

    // _cState.eip;  // TODO

    // _cState.ebp;  // TODO
    // _cState.esp;  // TODO
    _cState.ss = 0;

    _cState.cs = 0x8;
    _cState.ds = 0;

    _cState.es = 0;
    _cState.gs = 0;
    _cState.fs = 0;

    // _cState.eflags;  // TODO
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

/*
void Process::saveManagerState(){
    // save all registers
    asm volatile("mov %0, %%eax" : : "r"(_dispatcherState.eax));
    asm volatile("mov %0, %%ebx" : : "r"(_dispatcherState.ebx));
    asm volatile("mov %0, %%ecx" : : "r"(_dispatcherState.ecx));
    asm volatile("mov %0, %%edx" : : "r"(_dispatcherState.edx));

    // asm volatile("mov %0, %%esi" : : "r"(_dispatcherState.esi));
    // asm volatile("mov %0, %%edi" : : "r"(_dispatcherState.edi));

    // asm volatile("mov %0, %%eip" : : "r"(_dispatcherState.eip));

    // asm volatile("mov %0, %%ebp" : : "r"(_dispatcherState.ebp));
    // asm volatile("mov %0, %%esp" : : "r"(_dispatcherState.esp));
    asm volatile("mov %0, %%ss" : : "r"(_dispatcherState.ss));

    asm volatile("mov %0, %%cs" : : "r"(_dispatcherState.cs));
    asm volatile("mov %0, %%ds" : : "r"(_dispatcherState.ds));

    asm volatile("mov %0, %%es" : : "r"(_dispatcherState.es));
    asm volatile("mov %0, %%gs" : : "r"(_dispatcherState.gs));
    asm volatile("mov %0, %%fs" : : "r"(_dispatcherState.fs));

    // asm volatile("mov %0, %%eflags" : : "r"(_dispatcherState.eflags));

}
 */

void Process::pause() {
    // Not properly implemented yet
    return;

    _pState = Paused;

    // save all registers
    asm volatile("mov %0, %%eax" : : "r"(_cState.eax));
    asm volatile("mov %0, %%ebx" : : "r"(_cState.ebx));
    asm volatile("mov %0, %%ecx" : : "r"(_cState.ecx));
    asm volatile("mov %0, %%edx" : : "r"(_cState.edx));

    // asm volatile("mov %0, %%esi" : : "r"(_cState.esi));
    // asm volatile("mov %0, %%edi" : : "r"(_cState.edi));

    // asm volatile("mov %0, %%eip" : : "r"(_cState.eip));

    // asm volatile("mov %0, %%ebp" : : "r"(_cState.ebp));
    // asm volatile("mov %0, %%esp" : : "r"(_cState.esp));
    asm volatile("mov %0, %%ss" : : "r"(_cState.ss));

    asm volatile("mov %0, %%cs" : : "r"(_cState.cs));
    asm volatile("mov %0, %%ds" : : "r"(_cState.ds));

    asm volatile("mov %0, %%es" : : "r"(_cState.es));
    asm volatile("mov %0, %%gs" : : "r"(_cState.gs));
    asm volatile("mov %0, %%fs" : : "r"(_cState.fs));

    // asm volatile("mov %0, %%eflags" : : "r"(_cState.eflags));
}

void Process::resume() {
    // Not properly implemented yet
    return;

    _pState = Running;

    // restore all registers
    asm volatile("mov %%eax, %0" : : "r"(_cState.eax));
    asm volatile("mov %%ebx, %0" : : "r"(_cState.ebx));
    asm volatile("mov %%ecx, %0" : : "r"(_cState.ecx));
    asm volatile("mov %%edx, %0" : : "r"(_cState.edx));

    // asm volatile("mov %%esi, %0" : : "r"(_cState.esi));
    // asm volatile("mov %%edi, %0" : : "r"(_cState.edi));

    // asm volatile("mov %%ebp, %0" : : "r"(_cState.ebp));
    // asm volatile("mov %%esp, %0" : : "r"(_cState.esp));
    asm volatile("mov %%ss, %0" : : "r"(_cState.ss));

    asm volatile("mov %%cs, %0" : : "r"(_cState.cs));
    asm volatile("mov %%ds, %0" : : "r"(_cState.ds));

    asm volatile("mov %%es, %0" : : "r"(_cState.es));
    asm volatile("mov %%gs, %0" : : "r"(_cState.gs));
    asm volatile("mov %%fs, %0" : : "r"(_cState.fs));

    // asm volatile("mov %%eflags, %0" : : "r"(_cState.eflags));

    asm volatile("mov %%eip, %0" : : "r"(_cState.eip));
}
