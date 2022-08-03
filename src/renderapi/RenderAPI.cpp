#include "RenderAPI.h"
#include "opengl/OpenGLAPI.h"

static RenderAPI* currentAPI = nullptr;

RenderAPI* defaultRenderAPI() {
    if (currentAPI == nullptr) {
        currentAPI = new OpenGLAPI();
    }

    return currentAPI;
}

void deleteRenderAPI() {
    delete currentAPI;
    currentAPI = nullptr;
}
