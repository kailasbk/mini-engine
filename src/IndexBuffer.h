#ifndef OPENGL_RENDERER_INDEXBUFFER_H
#define OPENGL_RENDERER_INDEXBUFFER_H


class IndexBuffer {
public:
    /**
     * Constructs the index buffer.
     *
     * @param indices the pointer to the index buffer data
     * @param indexCount the number of indices in the buffer
     * @param indexSize the size of each index, in bytes (1, 2, or 4)
     */
    IndexBuffer(const void* indices, int indexCount, int indexSize);

    /**
     * Destructs the index buffer.
     */
    ~IndexBuffer();

    /**
     * Binds the index buffer.
     */
    void bind() const;

    /**
     * Unbinds the index buffer.
     */
    static void unbind();

    /**
     * @return the number of indices in the buffer
     */
    int getNumIndices() const;
private:
    unsigned int m_bufferID;
    const int m_indexCount;
};


#endif //OPENGL_RENDERER_INDEXBUFFER_H
