#ifndef OPENGL_RENDERER_RENDERER3D_H
#define OPENGL_RENDERER_RENDERER3D_H

#include "../rhi/RHI.h"
#include "StaticMesh.h"
#include "Camera3D.h"

/**
 * A 3d renderer that renders meshes to a framebuffer.
 */
class Renderer3D {
public:
    explicit Renderer3D(std::shared_ptr<const Camera3D> camera)
        : m_camera(std::move(camera)) {
        if (m_camera == nullptr) {
            throw std::invalid_argument("Renderer3D must have a camera.");
        }
    }

    /**
     * Sets the camera to base the renderers view from. Meshes are rendered from the camera's
     * point of view.
     *
     * @param camera the camera to view the meshes from
     */
    void setCamera(std::shared_ptr<const Camera3D> camera) {
        if (camera == nullptr) {
            throw std::invalid_argument("Renderer3D must have a camera.");
        }

        m_camera = std::move(camera);
    }

    /**
     * Begins rendering to the given framebuffer. This binds the framebuffer, clears attachments,
     * and sets the viewport to render into the framebuffer.
     *
     * @param framebuffer the framebuffer to render to, must not be nullptr
     */
    void begin(std::shared_ptr<Framebuffer> framebuffer);

    /**
     * Ends rendering to the framebuffer.
     */
    void end();

    /**
     * Submits a static mesh to be rendered with the given model transform.
     * This function should only be called between calls to begin() and end().
     *
     * @param mesh the mesh to render, must be renderable
     * @param transform the model transform for the mesh
     */
    void submit(const StaticMesh& mesh, const Mat4& transform);

private:
    std::shared_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<const Camera3D> m_camera;
};


#endif //OPENGL_RENDERER_RENDERER3D_H
