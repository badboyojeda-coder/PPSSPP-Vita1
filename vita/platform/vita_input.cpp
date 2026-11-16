#include "vita_input.h"
#include "vita_platform.h"

#include <psp2/ctrl.h>

#include "Common/Input/InputState.h"
#include "Common/Input/KeyCodes.h"
#include "Common/System/NativeApp.h"

struct VitaButtonMapping {
    unsigned int vitaButton;
    InputKeyCode ppssppKey;
};

static const VitaButtonMapping buttonMap[] = {
    {SCE_CTRL_CROSS, NKCODE_BUTTON_A},
    {SCE_CTRL_CIRCLE, NKCODE_BUTTON_B},
    {SCE_CTRL_SQUARE, NKCODE_BUTTON_X},
    {SCE_CTRL_TRIANGLE, NKCODE_BUTTON_Y},
    {SCE_CTRL_UP, NKCODE_DPAD_UP},
    {SCE_CTRL_DOWN, NKCODE_DPAD_DOWN},
    {SCE_CTRL_LEFT, NKCODE_DPAD_LEFT},
    {SCE_CTRL_RIGHT, NKCODE_DPAD_RIGHT},
    {SCE_CTRL_START, NKCODE_BUTTON_START},
    {SCE_CTRL_SELECT, NKCODE_BUTTON_SELECT},
    {SCE_CTRL_LTRIGGER, NKCODE_BUTTON_L1},
    {SCE_CTRL_RTRIGGER, NKCODE_BUTTON_R1},
    {SCE_CTRL_L3, NKCODE_BUTTON_THUMBL},
    {SCE_CTRL_R3, NKCODE_BUTTON_THUMBR},
};

static unsigned int previousButtons = 0;

static float normalizeStick(unsigned char value) {
    float axis = ((float)value - 128.0f) / 127.0f;

    if (axis > 1.0f)
        axis = 1.0f;

    if (axis < -1.0f)
        axis = -1.0f;

    const float deadzone = 0.08f;

    if (axis > -deadzone && axis < deadzone)
        return 0.0f;

    if (axis > 0.0f)
        return (axis - deadzone) / (1.0f - deadzone);

    return (axis + deadzone) / (1.0f - deadzone);
}

int vita_input_init(void) {
    previousButtons = 0;
    return vita_platform_init();
}

int vita_input_update(void) {
    VitaInputState state{};

    int result = vita_platform_poll_input(&state);

    if (result <= 0)
        return result;

    for (const auto &mapping : buttonMap) {
        bool pressed = (state.buttons & mapping.vitaButton) != 0;
        bool wasPressed = (previousButtons & mapping.vitaButton) != 0;

        if (pressed != wasPressed) {
            KeyInput key(
                DEVICE_ID_PAD_0,
                mapping.ppssppKey,
                pressed ? KeyInputFlags::DOWN : KeyInputFlags::UP
            );

            NativeKey(key);
        }
    }

    previousButtons = state.buttons;

    AxisInput axes[4] = {
        {DEVICE_ID_PAD_0, JOYSTICK_AXIS_X, normalizeStick(state.lx)},
        {DEVICE_ID_PAD_0, JOYSTICK_AXIS_Y, normalizeStick(state.ly)},
        {DEVICE_ID_PAD_0, JOYSTICK_AXIS_Z, normalizeStick(state.rx)},
        {DEVICE_ID_PAD_0, JOYSTICK_AXIS_RZ, normalizeStick(state.ry)},
    };

    NativeAxis(axes, 4);

    return result;
}
