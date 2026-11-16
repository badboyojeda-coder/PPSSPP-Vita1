#pragma once

#include "Common/GPU/OpenGL/OpenGLGraphicsContext.h"

class VitaGraphicsContext final : public OpenGLGraphicsContext {
public:
    bool InitSurface(WindowSystem winsys, void *data1, void *data2,
                     std::string *error_message) override;
    void ShutdownSurface() override;
};
