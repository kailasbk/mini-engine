#include "Renderer3D.h"

void Renderer3D::begin(std::shared_ptr<Framebuffer> framebuffer) {
    if (framebuffer == nullptr) {
        throw std::invalid_argument("Renderer3D requires a framebuffer to render to.");
    }
    m_framebuffer = std::move(framebuffer);

    // bind framebuffer for subsequent rendering
    RHI& rhi = RHI::current();
    rhi.bindFramebuffer(*m_framebuffer);

    // clear the framebuffer attachments and set viewport to entire framebuffer
    rhi.clearAttachments(0.12f, 0.12f, 0.12f, 1.0f, 1.0f); // TODO: remove magic number
    Vector<uint32_t, 2> dimensions = m_framebuffer->dimensions();
    rhi.setViewport(0, 0, dimensions.x, dimensions.y);
}

void Renderer3D::end() {
    // for now this renders nothing, but could wait till now to actually render
    m_framebuffer.reset();
}

void Renderer3D::submit(const StaticMesh& mesh, const Mat4& transform) {
    if (m_framebuffer == nullptr) {
        throw std::invalid_argument("Renderer3D requires a framebuffer to render to.");
    }

    RHI& rhi = RHI::current();

    // must be renderable
    if (!mesh.isRenderable()) {
        throw std::invalid_argument("StaticMesh must be renderable to submit.");
    }

    // bind the material with parameters
    Material& material = *(mesh.material);
    Mat4 modelViewProjection = m_camera->viewProjectionMatrix() * transform;
    material.setModelViewProjection(modelViewProjection);
    material.setLights({m_camera->position()});
    material.bind();

    // bind the vertex buffer
    Buffer& vertexBuffer = *(mesh.vertexBuffer);
    rhi.bindVertexBuffer(vertexBuffer, 0);

    // bind the index buffer, if applicable, then draw
    if (mesh.indexBuffer == nullptr) {
        rhi.draw(vertexBuffer.size() / vertexBuffer.stride(), 0);
    } else {
        Buffer& indexBuffer = *(mesh.indexBuffer);
        rhi.bindIndexBuffer(indexBuffer);
        rhi.drawIndexed(indexBuffer.size() / indexBuffer.stride(), 0, 0);
    }
}
