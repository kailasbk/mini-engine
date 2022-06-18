#include "VertexFormat.h"

#include <GL/glew.h>
#include "Math.h"

VertexFormat::VertexFormat(const std::vector<Attribute>& attributes)
        : m_arrayID(0) {
    glCreateVertexArrays(1, &m_arrayID);
    glBindVertexArray(m_arrayID);
    for (Attribute attr : attributes) {
        GLenum type;
        switch (attr.type) {
            case math::Float:
                type = GL_FLOAT;
                break;
            case math::Byte:
                type = GL_BYTE;
                break;
            case math::UByte:
                type = GL_UNSIGNED_BYTE;
                break;
            case math::Short:
                type = GL_SHORT;
                break;
            case math::UShort:
                type = GL_UNSIGNED_SHORT;
                break;
            case math::Int:
                type = GL_INT;
                break;
            case math::UInt:
                type = GL_UNSIGNED_INT;
                break;
        }
        glEnableVertexAttribArray(attr.index);
        glVertexAttribBinding(attr.index, attr.buffer);
        if (attr.asFloat) {
            glVertexAttribFormat(attr.index, attr.count, type, attr.normalized ? GL_TRUE : GL_FALSE, attr.offset);
        }
        else {
            glVertexAttribIFormat(attr.index, attr.count, type, attr.offset);
        }
    }
    glBindVertexArray(0);
}

VertexFormat::~VertexFormat() {
    glDeleteVertexArrays(1, &m_arrayID);
}

void VertexFormat::bind() const {
    glBindVertexArray(m_arrayID);
}

void VertexFormat::unbind() {
    glBindVertexArray(0);
}
