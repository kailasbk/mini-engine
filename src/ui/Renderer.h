#ifndef OPENGL_RENDERER_RENDERER_H
#define OPENGL_RENDERER_RENDERER_H

#include "RenderList.h"
#include "../rhi/RHI.h"

namespace ui {

    class Renderer {
    public:
        Renderer(float width, float height);

        /**
         * Sets the UI resolution, defined by the width and height of the UI coordinate space.
         * This is used to translate UI coordinates to normalized device coordinates.
         *
         * @param width the width of the resolution
         * @param height the height of the resolution
         */
        void setResolution(float width, float height);

        /**
         * Renders the UI components from a render list of UI primitives (rects, images, and texts).
         *
         * @param list the render list to pull ui primitives from.
         */
        void render(const RenderList& list);

    private:
        struct Vertex {
            float position[3];
            float texture[2];
        };

        float m_width;
        float m_height;
        std::unique_ptr<Buffer> m_indexBuffer;
        std::unique_ptr<Buffer> m_vertexBuffer;
        std::unique_ptr<Pipeline> m_imagePipeline;
        std::unique_ptr<Pipeline> m_rectPipeline;
    };

} // ui

#endif //OPENGL_RENDERER_RENDERER_H
