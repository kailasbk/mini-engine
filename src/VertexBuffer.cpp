#include "VertexBuffer.h"

#include "GL/glew.h"

VertexBuffer::VertexBuffer(const void* vertices, unsigned int size, int stride)
        : m_bufferID(0), m_stride(stride) {
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_bufferID);
}

void VertexBuffer::bind(unsigned int index, int offset) const {
    glBindVertexBuffer(index, m_bufferID, offset, m_stride);
}

void VertexBuffer::unbind(unsigned int index) {
    glBindVertexBuffer(index, 0, 0, 0);
}
