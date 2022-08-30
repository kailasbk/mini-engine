#include "Uniform.h"
#include "UniformVisitor.h"

template<>
void ScalarUniform<float>::accept(UniformVisitor& visitor) const {
    visitor.bindFloat(*this);
}

template<>
void VectorUniform<float, 2>::accept(UniformVisitor& visitor) const {
    visitor.bindVec2(*this);
}

template<>
void VectorUniform<float, 3>::accept(UniformVisitor& visitor) const {
    visitor.bindVec3(*this);
}

template<>
void VectorUniform<float, 4>::accept(UniformVisitor& visitor) const {
    visitor.bindVec4(*this);
}

template<>
void MatrixUniform<4, 4>::accept(UniformVisitor& visitor) const {
    visitor.bindMat4(*this);
}
