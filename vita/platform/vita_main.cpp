#include <cstdio>
#include <vector>

#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>

#include "Common/GPU/GraphicsContext.h"
#include "Common/System/NativeApp.h"
#include "Common/TimeUtil.h"
#include "Core/CmdLine.h"
#include "Core/EmuThread.h"

#include "vita_graphics_context.h"
#include "vita_input.h"
#include "vita_system.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // The VPK's assets directory is relative to app0:.
    sceIoChdir("app0:");

    TimeInit();

    vita_input_init();

    // Force the OpenGL backend and use a writable Vita memory-card path.
    const char *vitaArgv[] = {
        "ppsspp",
        "--graphics=opengl",
        "--memstick=ux0:data/PPSSPP",
    };
    constexpr int vitaArgc = sizeof(vitaArgv) / sizeof(vitaArgv[0]);

    CommandLineOptions options;
    const CommandLineParseResult parseResult =
        options.Parse(vitaArgc, vitaArgv, CmdLineMode::Application);

    if (parseResult != CommandLineParseResult::Continue) {
        sceKernelExitProcess(parseResult == CommandLineParseResult::Error ? 1 : 0);
        return 0;
    }

    NativeInit(vitaArgc, vitaArgv, options,
               "ux0:data/PPSSPP",
               "ux0:data/PPSSPP",
               "ux0:data/PPSSPP");

    VitaGraphicsContext graphicsContext;
    std::string errorMessage;

    if (!graphicsContext.InitAPI(nullptr, nullptr, &errorMessage)) {
        std::fprintf(stderr, "Vita graphics API initialization failed: %s\n",
                     errorMessage.c_str());
        NativeShutdown();
        sceKernelExitProcess(1);
        return 1;
    }

    if (!graphicsContext.InitSurface(
            WINDOWSYSTEM_NONE, nullptr, nullptr, &errorMessage)) {
        std::fprintf(stderr, "Vita graphics surface initialization failed: %s\n",
                     errorMessage.c_str());
        graphicsContext.ShutdownAPI();
        NativeShutdown();
        sceKernelExitProcess(1);
        return 1;
    }

    RunGraphicsLoop(
        &graphicsContext,
        new NativeApplication(),
        [](GraphicsContext *context) {
            NativeFrame(context);
            return true;
        },
        []() {
            // Input is polled on the render thread. The emulator receives
            // button and analog events through the existing Vita input code.
            vita_input_update();

            SceCtrlData pad{};
            if (sceCtrlPeekBufferPositive(0, &pad, 1) > 0) {
                const unsigned int exitCombo = SCE_CTRL_START | SCE_CTRL_SELECT;
                if ((pad.buttons & exitCombo) == exitCombo)
                    vita_system_request_exit();
            }

            return vita_system_should_exit();
        });

    // RunGraphicsLoop has completed NativeShutdownGraphics before returning.
    graphicsContext.ShutdownSurface();
    graphicsContext.ShutdownAPI();
    NativeShutdown();

    sceKernelExitProcess(0);
    return 0;
}
