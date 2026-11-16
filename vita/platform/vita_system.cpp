#include "vita_system.h"

#include <atomic>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

#include "Common/System/NativeApp.h"
#include "Common/System/System.h"

static std::atomic_bool g_vitaQuitRequested{false};

void vita_system_request_exit() {
    g_vitaQuitRequested.store(true);
}

bool vita_system_should_exit() {
    return g_vitaQuitRequested.load();
}

void System_Toast(std::string_view text) {
    std::fprintf(stderr, "%.*s\n", (int)text.size(), text.data());
}

void System_ShowKeyboard() {
}

void System_Vibrate(int length_ms) {
    (void)length_ms;
}

void System_LaunchUrl(LaunchUrlType urlType, std::string_view url) {
    (void)urlType;
    (void)url;
}

AudioBackend *System_CreateAudioBackend() {
    // Audio will initially use PPSSPP's legacy mixing path.
    return nullptr;
}

bool System_MakeRequest(SystemRequestType type, int requestId,
                        const std::string &param1, const std::string &param2,
                        int64_t param3, int64_t param4) {
    (void)requestId;
    (void)param1;
    (void)param2;
    (void)param3;
    (void)param4;

    switch (type) {
    case SystemRequestType::EXIT_APP:
        vita_system_request_exit();
        return true;
    default:
        return false;
    }
}

PermissionStatus System_GetPermissionStatus(SystemPermission permission) {
    (void)permission;
    return PERMISSION_STATUS_GRANTED;
}

void System_AskForPermission(SystemPermission permission) {
    (void)permission;
}

std::string System_GetProperty(SystemProperty prop) {
    switch (prop) {
    case SYSPROP_NAME:
        return "PS Vita";
    case SYSPROP_DEVICE_MODEL:
        return "PlayStation Vita";
    case SYSPROP_DEVICE_NAME:
        return "PlayStation Vita";
    case SYSPROP_COMPUTER_NAME:
        return "PS Vita";
    case SYSPROP_USER_DOCUMENTS_DIR:
        return "ux0:data/PPSSPP";
    case SYSPROP_BUILD_VERSION:
        return "Vita experimental";
    default:
        return "";
    }
}

std::vector<std::string> System_GetPropertyStringVec(SystemProperty prop) {
    (void)prop;
    return {};
}

int64_t System_GetPropertyInt(SystemProperty prop) {
    switch (prop) {
    case SYSPROP_DISPLAY_XRES:
        return 960;
    case SYSPROP_DISPLAY_YRES:
        return 544;
    case SYSPROP_DISPLAY_REFRESH_RATE:
        return 60;
    case SYSPROP_DISPLAY_COUNT:
        return 1;
    case SYSPROP_DEVICE_TYPE:
        return DEVICE_TYPE_MOBILE;
    case SYSPROP_AUDIO_SAMPLE_RATE:
        return 44100;
    default:
        return -1;
    }
}

float System_GetPropertyFloat(SystemProperty prop) {
    switch (prop) {
    case SYSPROP_DISPLAY_SAFE_INSET_LEFT:
    case SYSPROP_DISPLAY_SAFE_INSET_RIGHT:
    case SYSPROP_DISPLAY_SAFE_INSET_TOP:
    case SYSPROP_DISPLAY_SAFE_INSET_BOTTOM:
        return 0.0f;
    case SYSPROP_DISPLAY_REFRESH_RATE:
        return 60.0f;
    default:
        return -1.0f;
    }
}

bool System_GetPropertyBool(SystemProperty prop) {
    switch (prop) {
    case SYSPROP_HAS_BACK_BUTTON:
        return true;
    case SYSPROP_CAN_GET_FREE_SPACE_FAST:
        return true;
    default:
        return false;
    }
}

void System_Notify(SystemNotification notification) {
    (void)notification;
}

std::vector<std::string> System_GetCameraDeviceList() {
    return {};
}

bool System_AudioRecordingIsAvailable() {
    return false;
}
