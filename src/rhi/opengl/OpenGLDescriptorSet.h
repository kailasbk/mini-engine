#ifndef OPENGL_RENDERER_OPENGLDESCRIPTORSET_H
#define OPENGL_RENDERER_OPENGLDESCRIPTORSET_H

#include "../RHI.h"

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


#endif //OPENGL_RENDERER_OPENGLDESCRIPTORSET_H
