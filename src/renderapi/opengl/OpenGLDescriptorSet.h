#ifndef OPENGL_RENDERER_OPENGLDESCRIPTORSET_H
#define OPENGL_RENDERER_OPENGLDESCRIPTORSET_H

#include "../RenderAPI.h"

struct OpenGLDescriptor {
    DescriptorType type;
    union {
        struct BufferInfo {
            GLuint handle;
            uint32_t offset;
            uint32_t range;
        } bufferInfo;
        struct TextureInfo {
            GLuint handle;
        } textureInfo;
    };
};

class OpenGLDescriptorSet : public DescriptorSet {
public:
    explicit OpenGLDescriptorSet(std::map<uint32_t, DescriptorType> bindings) : m_bindings(std::move(bindings)) {}

    void bindTexture2D(uint32_t binding, Texture2D& texture2D) override;
    void bindUniformBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) override;
    void bindStorageBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) override;

    const std::map<uint32_t, OpenGLDescriptor>& descriptors() const {
        return m_descriptors;
    }

    static OpenGLDescriptorSet& from(DescriptorSet& descriptorSet) {
        return dynamic_cast<OpenGLDescriptorSet&>(descriptorSet);
    }

private:
    std::map<uint32_t, DescriptorType> m_bindings;
    std::map<uint32_t, OpenGLDescriptor> m_descriptors;
};

#if 0
enum class DescriptorType {
    Texture2D, UniformBuffer, StorageBuffer
};

class DescriptorSet {
    /**
     * Creates a descriptor binding the given 2d texture at the given index.
     *
     * @param binding the binding index of the descriptor
     * @param texture2D the 2d texture to bind
     * @returns the constructed descriptor
     */
static Descriptor bindTexture2D(uint32_t binding, const Texture2D& texture2D) {
    return {binding, std::addressof(texture2D)};
};

/**
 * Creates a descriptor binding the given uniform buffer at the given index.
 *
 * @param binding the binding index of the descriptor
 * @param buffer the uniform buffer to bind
 * @param offset the offset into the buffer to bind, in bytes
 * @param range the range of the buffer to bind, in bytes
 * @returns the constructed descriptor
 */
static Descriptor bindUniformBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) {
    return {binding, DescriptorType::UniformBuffer, std::addressof(buffer), offset, range};
}

/**
 * Creates a descriptor binding the given storage buffer at the given index.
 *
 * @param binding the binding index of the descriptor
 * @param buffer the storage buffer to bind
 * @param offset the offset into the buffer to bind, in bytes
 * @param range the range of the buffer to bind, in bytes
 * @returns the constructed descriptor
 */
static Descriptor bindStorageBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) {
    return {binding, DescriptorType::StorageBuffer, std::addressof(buffer), offset, range};
}

DescriptorType type;
uint32_t binding;
union {
    struct BufferInfo {
        const Buffer* buffer;
        uint32_t offset;
        uint32_t range;
    } bufferInfo;
    struct TextureInfo {
        const Texture2D* texture2D;
    } textureInfo;
};

private:
Descriptor(uint32_t binding, const Texture2D* texture2D)
: type(DescriptorType::Texture2D), binding(binding), textureInfo({texture2D}) {}

Descriptor(uint32_t binding, DescriptorType type, const Buffer* buffer, uint32_t offset, uint32_t range)
: type(type), binding(binding), bufferInfo({buffer, offset, range}) {}
};

class DescriptorSet {
public:
    explicit DescriptorSet(std::vector<Descriptor> descriptors) : m_descriptors(std::move(descriptors)) {};

private:
    std::vector<Descriptor> m_descriptors;
};
#endif

#endif //OPENGL_RENDERER_OPENGLDESCRIPTORSET_H
