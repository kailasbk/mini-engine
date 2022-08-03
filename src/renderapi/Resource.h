#ifndef OPENGL_RENDERER_RESOURCE_H
#define OPENGL_RENDERER_RESOURCE_H


template<typename T>
class Resource {
public:
    Resource(const Resource&) = delete;
    explicit Resource(T handle) : m_handle(handle) {}

    /**
     * @returns the platform-specific handle to the resource
     */
    T handle() const noexcept {
        return m_handle;
    }
protected:
    T m_handle;
};


#endif //OPENGL_RENDERER_RESOURCE_H
