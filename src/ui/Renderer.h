#pragma once

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
    void set_resolution(float width, float height);

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

    float width_;
    float height_;
    std::unique_ptr<Buffer> index_buffer_;
    std::unique_ptr<Buffer> vertex_buffer_;
    std::unique_ptr<Pipeline> image_pipeline_;
    std::unique_ptr<Pipeline> rect_pipeline_;
};

} // ui
