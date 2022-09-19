#ifndef OPENGL_RENDERER_RHI_H
#define OPENGL_RENDERER_RHI_H

#include "Buffer.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Pipeline.h"
#include "Framebuffer.h"
#include "VertexLayout.h"
#include "Uniform.h"
#include "DescriptorSet.h"

/**
 * Base class for platform-specific render api implementations.
 */
class RHI {
public:
    virtual ~RHI() = default;

    /**
     * Creates a buffer object of the given size.
     *
     * @param size the size of the buffer, in bytes
     * @param stride the stride (distance between elements) of the buffer, in bytes
     * @returns the constructed buffer object
     */
    virtual std::unique_ptr<Buffer> createBuffer(uint32_t size, uint32_t stride) = 0;

    /**
     * Creates a 2d texture object with the given width and height.
     *
     * @param format the format for the texture's pixels
     * @param width the width of the texture, in pixels
     * @param height the height of the texture, in pixels
     * @returns the constructed texture object
     */
    virtual std::unique_ptr<Texture2D> createTexture2D(Format format, uint32_t width, uint32_t height) = 0;

    /**
     * Creates a shader module for the given stage from the given code.
     *
     * @param code the code to be used to generate the shader
     * @param codeSize the size of the code, in bytes
     * @param type the type of the shader
     * @returns the constructed shader module
     */
    virtual std::unique_ptr<Shader> createShader(const void* code, size_t codeSize, ShaderType type) = 0;

    /**
     * Creates a descriptor set using the given bindings.
     *
     * @param bindings the binding of binding index to descriptor type for the set
     * @returns the constructed descriptor set
     */
    virtual std::unique_ptr<DescriptorSet> createDescriptorSet(std::vector<DescriptorSetBinding> bindings) = 0;

    /**
     * @returns a builder for creating pipelines
     */
    virtual std::unique_ptr<PipelineBuilder> createPipelineBuilder() = 0;

    /**
     * @returns a builder for creating framebuffers
     */
    virtual std::unique_ptr<FramebufferBuilder> createFramebufferBuilder() = 0;

    /**
     * Transfers pixel data from a buffer to a 2d texture, overwriting the texture completely.
     * All mip-map levels are regenerated afterward.
     *
     * @param source the source buffer, must not be mapped
     * @param destination the destination texture
     */
    virtual void copyBufferToTexture2D(Buffer& source, Texture2D& destination) = 0;

    /**
     * Resolves a multi-sampled 2d texture into a single-sampled texture.
     *
     * @param source the source multi-sampled texture
     * @param destination the destination single-sampled texture
     */
    virtual void resolveTexture2D(Texture2D& source, Texture2D& destination) = 0;

    /**
     * Binds a buffer for use as a vertex buffer at the given binding location.
     *
     * @param buffer the vertex buffer to bind
     * @param binding the binding location
     */
    virtual void bindVertexBuffer(const Buffer& buffer, uint32_t binding) = 0;

    /**
     * Binds a buffer for use as the index buffer.
     *
     * @param buffer the index buffer to bind
     */
    virtual void bindIndexBuffer(const Buffer& buffer) = 0;

    /**
     * Binds a uniform block for sourcing the default uniform block for draw calls. If the uniform block's
     * values change, this function must be called again to update the values bound. This function should be
     * called after binding the pipeline using bindPipeline().
     *
     * @param uniformBlock the uniform block to bind
     */
    virtual void bindUniforms(UniformBlock& uniformBlock) = 0;

    /**
     * Binds a descriptor set for sourcing texture and buffer bindings for draw calls.
     *
     * @param descriptorSet the descriptor set to bind
     */
    virtual void bindDescriptorSet(const DescriptorSet& descriptorSet) = 0;

    /**
     * Binds a pipeline for subsequent draw calls.
     *
     * @param pipeline the pipeline to bind
     */
    virtual void bindPipeline(const Pipeline& pipeline) = 0;

    virtual void bindFramebuffer(Framebuffer& framebuffer) = 0;
    virtual void bindDefaultFramebuffer() = 0;

    // set rendering parameters
    /**
     * Sets the viewport for subsequent draw calls to the currently bound framebuffer. The coordinates are
     * given in pixels (+x is to the right, +y is downward) and the point (0, 0) is the upper-left corner.
     *
     * @param x the x coordinate of the upper-left corner of the viewport
     * @param y the y coordinate of the upper-left corner of the viewport
     * @param width the width of the viewport, in pixels
     * @param height the height of the viewport, in pixels
     */
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    /**
     * Clears the attachments of the currently bound framebuffer.
     *
     * @param r the red component of the color clear value
     * @param g the green component of the color clear value
     * @param b the blue component of the color clear value
     * @param a the alpha component of the color clear value
     * @param depth the depth clear value
     */
    virtual void clearAttachments(float r, float g, float b, float a, float depth) = 0;

    // draw calls
    /**
     * Draws primitives with direct vertices using the given parameters.
     *
     * @param vertexCount the number of vertices to draw
     * @param baseVertex the offset of the 0th vertex, in elements
     */
    virtual void draw(uint32_t vertexCount, uint32_t baseVertex) = 0;

    /**
     * Draws primitives with indexed vertices using the given parameters.
     *
     * @param indexCount the number of indices to draw
     * @param baseIndex the offset of the 0th index, in elements
     * @param baseVertex the offset of the 0th vertex, in elements
     */
    virtual void drawIndexed(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) = 0;

    /**
     * Sets the current api to be the default render api for the platform.
     */
    static void create();

    /**
     * @returns the current api that is in use
     * @throws std::runtime_error if an api has not been created
     */
    static RHI& current();

    /**
     * Destroys the current api.
     */
    static void destroy();

private:
    static std::unique_ptr<RHI> currentAPI;
};


#endif //OPENGL_RENDERER_RHI_H
