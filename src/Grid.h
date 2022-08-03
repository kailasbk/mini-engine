#ifndef OPENGL_RENDERER_GRID_H
#define OPENGL_RENDERER_GRID_H

#include "renderapi/RenderAPI.h"

class Grid {
public:
    /**
     * Constructs a 2*size by 2*getNumIndices square grid with tiles of tileSize by tileSize.
     *
     * @param size the width of the grid
     * @param tileSize the width of the grid tiles
     */
    Grid(float size, float tileSize);

    const Buffer& vertexBuffer() const {
        return *m_buffer;
    };

    Pipeline& pipeline() const {
        return *m_pipeline;
    };

    uint32_t numVertices() const {
        return m_numVertices;
    };

private:
    uint32_t m_numVertices;

    std::unique_ptr<Pipeline> m_pipeline;
    std::unique_ptr<Buffer> m_buffer;
};


#endif //OPENGL_RENDERER_GRID_H
