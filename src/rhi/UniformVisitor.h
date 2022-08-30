#ifndef OPENGL_RENDERER_UNIFORMVISITOR_H
#define OPENGL_RENDERER_UNIFORMVISITOR_H

#include "../util/Vector.h"
#include "../util/Matrix.h"
#include "Uniform.h"

class UniformVisitor {
public:
    virtual ~UniformVisitor() = default;

    virtual void bindFloat(const ScalarUniform<float>& uniform) = 0;
    virtual void bindVec2(const VectorUniform<float, 2>& uniform) = 0;
    virtual void bindVec3(const VectorUniform<float, 3>& uniform) = 0;
    virtual void bindVec4(const VectorUniform<float, 4>& uniform) = 0;
    virtual void bindMat4(const MatrixUniform<4, 4>& uniform) = 0;
};


#endif //OPENGL_RENDERER_UNIFORMVISITOR_H
