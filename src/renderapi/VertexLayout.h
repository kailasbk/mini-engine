#ifndef OPENGL_RENDERER_VERTEXLAYOUT_H
#define OPENGL_RENDERER_VERTEXLAYOUT_H

#include "Format.h"

struct VertexAttribute {
    VertexAttribute(uint32_t location, Format format, uint32_t offset)
            : location(location), format(format), offset(offset) {}

    const uint32_t location;
    const Format format;
    const uint32_t offset;
};

struct VertexBinding {
    VertexBinding(uint32_t binding, uint32_t stride, std::vector<VertexAttribute>&& attributes)
            : binding(binding), stride(stride), attributes(std::move(attributes)) {}

    const uint32_t binding;
    const uint32_t stride;
    const std::vector<VertexAttribute> attributes;
    // TODO: add per vertex / per instance setting
};

struct VertexLayout {
    explicit VertexLayout(std::vector<VertexBinding>&& bindings)
            : bindings(std::move(bindings)) {};
    const std::vector<VertexBinding> bindings;
};


#endif //OPENGL_RENDERER_VERTEXLAYOUT_H
