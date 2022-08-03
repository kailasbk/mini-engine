#include "OpenGLBuffer.h"

void* OpenGLBuffer::map(uint32_t offset, uint32_t size) {
    m_isMapped = true;
    return glMapNamedBufferRange(m_handle, offset, size,
                                 GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
}

void OpenGLBuffer::unmap() {
    glUnmapNamedBuffer(m_handle);
    m_isMapped = false;
}

bool OpenGLBuffer::isMapped() const {
    return m_isMapped;
}

std::unique_ptr<Buffer> OpenGLAPI::createBuffer(uint32_t size, uint32_t stride) {
    GLuint handle;
    glCreateBuffers(1, &handle);
    glNamedBufferStorage(handle, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    return std::make_unique<OpenGLBuffer>(handle, size, stride);
}