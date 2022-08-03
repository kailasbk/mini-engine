#ifndef OPENGL_RENDERER_OPENGLUNIFORMVISITOR_H
#define OPENGL_RENDERER_OPENGLUNIFORMVISITOR_H

#include "../UniformVisitor.h"

class OpenGLUniformVisitor : public UniformVisitor {
public:
    OpenGLUniformVisitor() : m_location(0) {};

    void setLocation(uint32_t location) {
        m_location = location;
    }

    void bindFloat(const ScalarUniform<float>& uniform) override;

    void bindVec3(const VectorUniform<float, 3>& uniform) override;

    void bindMat4(const MatrixUniform<4, 4>& uniform) override;

private:
    uint32_t m_location;
};


#endif //OPENGL_RENDERER_OPENGLUNIFORMVISITOR_H
