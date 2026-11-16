#include "vita_graphics_context.h"

#include <vitaGL.h>

bool VitaGraphicsContext::InitSurface(WindowSystem winsys, void *data1,
                                      void *data2, std::string *error_message) {
    (void)winsys;
    (void)data1;
    (void)data2;

    if (!vglInit(0x800000)) {
        if (error_message)
            *error_message = "vitaGL initialization failed";
        return false;
    }

    if (!OpenGLGraphicsContext::InitSurface(
            WINDOWSYSTEM_NONE, nullptr, nullptr, error_message)) {
        return false;
    }

    renderManager_->SetSwapFunction([]() {
        vglSwapBuffers(GL_FALSE);
    });

    return true;
}

void VitaGraphicsContext::ShutdownSurface() {
    // VitaGL has no public shutdown function in the API currently used.
    // Destroy PPSSPP GL resources while the VitaGL context is still active.
    OpenGLGraphicsContext::ShutdownSurface();
}
