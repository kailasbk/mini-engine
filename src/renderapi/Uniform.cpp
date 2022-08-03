#include "Uniform.h"
#include "UniformVisitor.h"

template<>
void ScalarUniform<float>::bind(UniformVisitor& visitor) const {
    visitor.bindFloat(*this);
}

template<>
void VectorUniform<float, 3>::bind(UniformVisitor& visitor) const {
    visitor.bindVec3(*this);
}

template<>
void MatrixUniform<4, 4>::bind(UniformVisitor& visitor) const {
    visitor.bindMat4(*this);
}
