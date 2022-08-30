#include "RHI.h"
#include "opengl/OpenGLRHI.h"

std::unique_ptr<RHI> RHI::currentAPI(nullptr);

void RHI::create() {
    currentAPI = std::make_unique<OpenGLRHI>();
}

RHI& RHI::current() {
    if (currentAPI == nullptr) {
        throw std::runtime_error("there is not current render api.");
    }

    return *currentAPI;
}

void RHI::destroy() {
    currentAPI.reset(nullptr);
}
