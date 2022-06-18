#include "IndexBuffer.h"

#include <GL/glew.h>
#include <iostream>

IndexBuffer::IndexBuffer(const void* indices, int indexCount, int indexSize)
        : m_bufferID(0), m_indexCount(indexCount) {
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, indexCount * indexSize, indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_bufferID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int IndexBuffer::getNumIndices() const {
    return m_indexCount;
}
