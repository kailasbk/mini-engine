#ifndef OPENGL_RENDERER_VERTEXFORMAT_H
#define OPENGL_RENDERER_VERTEXFORMAT_H

#include <vector>
#include "Math.h"

class VertexFormat {
public:
    struct Attribute {
        unsigned int index; // the index of the attribute
        unsigned int buffer; // the binding index of the buffer
        math::ScalarType type; // type of the element
        int count; // number of elements (1, 2, 3 or 4)
        bool asFloat; // whether used by shader as float
        bool normalized; // whether to normalize
        int offset; // offset from start of struct, in bytes
    };

    /**
     * Constructs the vertex format from the given attributes.
     *
     * @param attributes the vertex attributes
     */
    explicit VertexFormat(const std::vector<Attribute>& attributes);

    /**
     * Destructs the vertex format.
     */
    ~VertexFormat();

    /**
     * Binds the vertex format.
     */
    void bind() const;

    /**
     * Unbinds the vertex format.
     */
    static void unbind();
private:
    unsigned int m_arrayID;
};


#endif //OPENGL_RENDERER_VERTEXFORMAT_H
