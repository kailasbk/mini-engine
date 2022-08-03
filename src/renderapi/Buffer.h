#ifndef OPENGL_RENDERER_BUFFER_H
#define OPENGL_RENDERER_BUFFER_H


class Buffer {
public:
    explicit Buffer(uint32_t size, uint32_t stride) : m_size(size), m_stride(stride) {}

    virtual ~Buffer() = default;

    /**
     * @returns the size of the buffer, in bytes
     */
    uint32_t size() const {
        return m_size;
    }

    /**
     * @returns the stride of the buffer, in bytes
     */
    uint32_t stride() const {
        return m_stride;
    }

    /**
     * Maps the entire buffer to host memory through which it can be written to.
     *
     * @returns a pointer to mapped region of host memory
     */
    void* map() {
        return map(0, m_size);
    }

    /**
     * Maps a region of the buffer to host memory through which it can be written to.
     *
     * @param offset the offset of region into the buffer, in bytes
     * @param size the size of the region, in bytes
     * @returns a pointer to mapped region of host memory
     */
    virtual void* map(uint32_t offset, uint32_t size) = 0;

    /**
     * Unmaps the buffer, invalidating the pointer returned from map().
     */
    virtual void unmap() = 0;

    /**
     * @returns whether the buffer is currently mapped
     */
    virtual bool isMapped() const = 0;

private:
    uint32_t m_size;
    uint32_t m_stride;
};


#endif //OPENGL_RENDERER_BUFFER_H
