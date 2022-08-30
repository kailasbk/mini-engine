#ifndef OPENGL_RENDERER_DESCRIPTORSET_H
#define OPENGL_RENDERER_DESCRIPTORSET_H

#include "Texture2D.h"
#include "Buffer.h"

enum class DescriptorType {
    Texture2D, UniformBuffer, StorageBuffer
};

struct DescriptorSetBinding {
    const uint32_t binding;
    const DescriptorType type;
};

class DescriptorSet {
public:
    DescriptorSet() = default;
    DescriptorSet(const DescriptorSet&) = delete;
    virtual ~DescriptorSet() = default;

    /**
     * Creates a descriptor binding the given 2d texture at the given index.
     *
     * @param binding the binding index of the descriptor
     * @param texture2D the 2d texture to bind
     * @returns the constructed descriptor
     */
    virtual void bindTexture2D(uint32_t binding, Texture2D& texture2D) = 0;

    /**
     * Creates a descriptor binding the given uniform buffer at the given index.
     *
     * @param binding the binding index of the descriptor
     * @param buffer the uniform buffer to bind
     * @param offset the offset into the buffer to bind, in bytes
     * @param range the range of the buffer to bind, in bytes
     * @returns the constructed descriptor
     */
    virtual void bindUniformBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) = 0;

    /**
     * Creates a descriptor binding the given storage buffer at the given index.
     *
     * @param binding the binding index of the descriptor
     * @param buffer the storage buffer to bind
     * @param offset the offset into the buffer to bind, in bytes
     * @param range the range of the buffer to bind, in bytes
     * @returns the constructed descriptor
     */
    virtual void bindStorageBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) = 0;
};


#endif //OPENGL_RENDERER_DESCRIPTORSET_H
