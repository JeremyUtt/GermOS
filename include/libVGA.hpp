#pragma once
int main2(int arg_c, char *arg_v[]);
// void dump_state(void);

enum UiMode {
    GRAPHICS,TEXT
};

void setUiMode(UiMode mode);
UiMode getUiMode();