#include "OpenGLUniformVisitor.h"

void OpenGLUniformVisitor::bindFloat(const ScalarUniform<float>& uniform) {
    glUniform1f((GLint)m_location, *(GLfloat*)uniform.data());
}

void OpenGLUniformVisitor::bindVec3(const VectorUniform<float, 3>& uniform) {
    glUniform3fv((GLint)m_location, 1, (const GLfloat*)uniform.data());
}

void OpenGLUniformVisitor::bindMat4(const MatrixUniform<4, 4>& uniform) {
    glUniformMatrix4fv((GLint)m_location, 1, GL_FALSE, (const GLfloat*)uniform.data());
}
