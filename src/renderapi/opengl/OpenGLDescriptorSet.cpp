#include "OpenGLDescriptorSet.h"
#include "OpenGLTexture2D.h"
#include "OpenGLBuffer.h"

std::unique_ptr<DescriptorSet> OpenGLAPI::createDescriptorSet(std::vector<DescriptorSetBinding> bindings) {
    std::map<uint32_t, DescriptorType> bindingsMap;

    for (auto binding : bindings) {
        bindingsMap[binding.binding] = binding.type;
    }

    return std::make_unique<OpenGLDescriptorSet>(std::move(bindingsMap));
}

void OpenGLDescriptorSet::bindTexture2D(uint32_t binding, Texture2D& texture2D) {
    if (!m_bindings.contains(binding) || m_bindings[binding] != DescriptorType::Texture2D) {
        throw std::invalid_argument("Binding index does not accept Texture2D");
    }

    OpenGLTexture2D& glTexture2D = OpenGLTexture2D::from(texture2D);
    OpenGLDescriptor descriptor{};
    descriptor.type = DescriptorType::Texture2D;
    descriptor.textureInfo.handle = glTexture2D.handle();
    m_descriptors[binding] = descriptor;
}

void OpenGLDescriptorSet::bindUniformBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) {
    if (m_bindings.at(binding) != DescriptorType::UniformBuffer) {
        throw std::invalid_argument("Binding index does not accept UniformBuffer");
    }

    const OpenGLBuffer& glBuffer = OpenGLBuffer::from(buffer);
    OpenGLDescriptor descriptor{};
    descriptor.type = DescriptorType::UniformBuffer;
    descriptor.bufferInfo.handle = glBuffer.handle();
    descriptor.bufferInfo.offset = offset;
    descriptor.bufferInfo.range = range;
    m_descriptors[binding] = descriptor;
}

void OpenGLDescriptorSet::bindStorageBuffer(uint32_t binding, const Buffer& buffer, uint32_t offset, uint32_t range) {
    if (m_bindings.at(binding) != DescriptorType::StorageBuffer) {
        throw std::invalid_argument("Binding index does not accept StorageBuffer");
    }

    const OpenGLBuffer& glBuffer = OpenGLBuffer::from(buffer);
    OpenGLDescriptor descriptor{};
    descriptor.type = DescriptorType::StorageBuffer;
    descriptor.bufferInfo.handle = glBuffer.handle();
    descriptor.bufferInfo.offset = offset;
    descriptor.bufferInfo.range = range;
    m_descriptors[binding] = descriptor;
}
