#include <psp2/ctrl.h>
#include "vita_platform.h"

int vita_platform_init(void) {
    return sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
}

int vita_platform_poll_input(VitaInputState *state) {
    if (state == 0) {
        return -1;
    }

    SceCtrlData input;
    int result = sceCtrlPeekBufferPositive(0, &input, 1);

    if (result <= 0) {
        return result;
    }

    state->buttons = input.buttons;
    state->lx = input.lx;
    state->ly = input.ly;
    state->rx = input.rx;
    state->ry = input.ry;

    return result;
}
