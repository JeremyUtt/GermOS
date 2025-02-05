#pragma once


enum UiMode {
    GRAPHICS,TEXT
};

void setUiMode(UiMode mode);
UiMode getUiMode();

void dump_state(void);