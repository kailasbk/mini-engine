#ifndef OPENGL_RENDERER_VERTEXBUFFER_H
#define OPENGL_RENDERER_VERTEXBUFFER_H


class VertexBuffer {
public:
    /**
     * Constructs the vertex buffer.
     *
     * @param vertices the pointer to the vertex buffer data
     * @param size the getNumIndices of the buffer data, in bytes
     * @param stride the getNumIndices of each vertex, in bytes
     */
    VertexBuffer(const void* vertices, unsigned int size, int stride);

    /**
     * Destructs the vertex buffer.
     */
    ~VertexBuffer();

    /**
     * Binds the vertex buffer, offset by the given offset, to the given binding index.
     *
     * @param index the binding index, must be less than 16
     */
    void bind(unsigned int index = 0, int offset = 0) const;

    /**
     * Unbinds the vertex buffer at the given binding index.
     *
     * @param index the binding index, must be less than 16
     */
    static void unbind(unsigned int index = 0);
private:
    unsigned int m_bufferID;
    int m_stride;
};


#endif //OPENGL_RENDERER_VERTEXBUFFER_H
