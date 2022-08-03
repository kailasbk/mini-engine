#include "OpenGLAPI.h"
#include "OpenGLBuffer.h"
#include "OpenGLTexture2D.h"
#include "OpenGLShader.h"
#include "OpenGLPipeline.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLUniformVisitor.h"
#include "OpenGLDescriptorSet.h"

void OpenGLAPI::copyBufferToTexture2D(Buffer& source, Texture2D& destination) {
    OpenGLBuffer& glSource = OpenGLBuffer::from(source);
    OpenGLTexture2D& glDest = OpenGLTexture2D::from(destination);

    // TODO: maybe assert that the source is at least as big as needed

    // copy data from buffer to texture's lod 0
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, glSource.handle());
    Vector<uint32_t, 3> size = destination.dimensions();
    glTextureSubImage2D(glDest.handle(), 0, // copy to level 0 of texture
                        0, 0, // starting from (0, 0)
                        (GLint)size.x, (GLint)size.y, // to (width, height)
                        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    // regenerate mipmaps for texture
    glGenerateTextureMipmap(glDest.handle());
}

void OpenGLAPI::resolveTexture2D(Texture2D& source, Texture2D& destination) {
    throw std::runtime_error("resolveTexture2D() is not implemented.");
}

void OpenGLAPI::bindVertexBuffer(const Buffer& buffer, uint32_t binding) {
    // assure that buffer stride matching the binding stride
    // assert(buffer.stride() == 0000);

    const OpenGLBuffer& glBuffer = OpenGLBuffer::from(buffer);

    // TODO: support for binding with offset (atm assumed to be 0)
    glBindVertexBuffer(binding, glBuffer.handle(), 0, (GLint)glBuffer.stride());
}

void OpenGLAPI::bindIndexBuffer(const Buffer& buffer) {
    // only support buffers with stride of 2 and 4 (uint16 and uint32)
    assert(buffer.stride() == 2 || buffer.stride() == 4);

    const OpenGLBuffer& glBuffer = OpenGLBuffer::from(buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer.handle());
    m_binds.indexBuffer = std::addressof(buffer);
}

void OpenGLAPI::bindUniforms(UniformBlock& uniforms) {
    OpenGLUniformVisitor visitor;
    for (int i = 0; i < uniforms.numFields(); i++) {
        visitor.setLocation(i);
        uniforms[i].bind(visitor);
    }
}

void OpenGLAPI::bindDescriptors(DescriptorSet& descriptorSet) {
    OpenGLDescriptorSet& glDescriptorSet = OpenGLDescriptorSet::from(descriptorSet);

    for (auto& descriptorPair: glDescriptorSet.descriptors()) {
        uint32_t binding = descriptorPair.first;
        OpenGLDescriptor descriptor = descriptorPair.second;

        switch (descriptor.type) {
            case DescriptorType::Texture2D:
                glBindTextureUnit(binding, descriptor.textureInfo.handle);
                break;
            case DescriptorType::UniformBuffer:
                glBindBufferRange(GL_UNIFORM_BUFFER, binding, descriptor.bufferInfo.handle,
                                  descriptor.bufferInfo.offset, descriptor.bufferInfo.range);
                break;
            case DescriptorType::StorageBuffer:
                glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, descriptor.bufferInfo.handle,
                                  descriptor.bufferInfo.offset, descriptor.bufferInfo.range);
                break;
        }
    }
}

void OpenGLAPI::bindPipeline(Pipeline& pipeline) {
    OpenGLPipeline& glPipeline = OpenGLPipeline::from(pipeline);

    for (const auto& binding: glPipeline.vertexLayout().bindings) {
        for (auto attribute: binding.attributes) {
            glEnableVertexAttribArray(attribute.location);
            glVertexAttribBinding(attribute.location, binding.binding);
            switch (attribute.format) {
                case Format::RGB8:
                case Format::RGBA8:
                    throw std::runtime_error("RGB(A)8 format not supported");
                    break;
                case Format::RG32F:
                    glVertexAttribFormat(attribute.location, 2, GL_FLOAT, GL_FALSE, attribute.offset);
                    break;
                case Format::RGB32F:
                    glVertexAttribFormat(attribute.location, 3, GL_FLOAT, GL_FALSE, attribute.offset);
                    break;
                case Format::RGBA32F:
                    glVertexAttribFormat(attribute.location, 4, GL_FLOAT, GL_FALSE, attribute.offset);
                    break;
            }
        }
    }

    glUseProgram(glPipeline.handle());
    m_binds.pipeline = std::addressof(pipeline);
}

void OpenGLAPI::bindFramebuffer(Framebuffer& framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, OpenGLFramebuffer::from(framebuffer).handle());
}

void OpenGLAPI::bindDefaultFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    // TODO: atm does not match the exact spec
    glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
}

void OpenGLAPI::clearAttachments(float r, float g, float b, float a, float depth) {
    glClearColor(r, g, b, a);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLAPI::draw(uint32_t vertexCount, uint32_t baseVertex) {
    if (m_binds.pipeline == nullptr) {
        throw std::domain_error("A pipeline must be bound for draw calls.");
    }

    GLenum mode;
    switch (m_binds.pipeline->topology()) {
        case Topology::Points:
            mode = GL_POINTS;
            break;
        case Topology::Lines:
            mode = GL_LINES;
            break;
        case Topology::Triangles:
            mode = GL_TRIANGLES;
            break;
    }

    glDrawArrays(mode, (GLint)baseVertex, (GLsizei)vertexCount);
}

void OpenGLAPI::drawIndexed(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) {
    if (m_binds.indexBuffer == nullptr || m_binds.pipeline == nullptr) {
        throw std::domain_error("An index buffer and pipeline must be bound for indexed draw calls.");
    }

    GLenum mode;
    switch (m_binds.pipeline->topology()) {
        case Topology::Points:
            mode = GL_POINTS;
            break;
        case Topology::Lines:
            mode = GL_LINES;
            break;
        case Topology::Triangles:
            mode = GL_TRIANGLES;
            break;
    }

    uint32_t indexStride = m_binds.indexBuffer->stride();
    GLenum type;
    switch (indexStride) {
        case 2:
            type = GL_UNSIGNED_SHORT;
            break;
        case 4:
            type = GL_UNSIGNED_INT;
            break;
        default:
            throw std::domain_error("Only index sizes of 2 and 4 bytes are supported.");
            break;
    }
    GLvoid* indices = (GLvoid*)(uintptr_t)(indexStride * baseIndex);

    glDrawElementsBaseVertex(mode, (GLsizei)indexCount, type, indices, (GLsizei)baseVertex);
}
