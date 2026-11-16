#pragma once

typedef struct {
    unsigned int buttons;
    unsigned char lx;
    unsigned char ly;
    unsigned char rx;
    unsigned char ry;
} VitaInputState;

int vita_platform_init(void);
int vita_platform_poll_input(VitaInputState *state);
