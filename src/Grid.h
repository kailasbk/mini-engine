#ifndef OPENGL_RENDERER_GRID_H
#define OPENGL_RENDERER_GRID_H

#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "Program.h"

#include <vector>

class Grid {
public:
    /**
     * Constructs a 2*size by 2*getNumIndices square grid with tiles of tileSize by tileSize.
     *
     * @param size the width of the grid
     * @param tileSize the width of the grid tiles
     */
    Grid(float size, float tileSize);

    const VertexBuffer& getVertexBuffer();
    const VertexFormat& getVertexFormat();
    const Program& getProgram();
    int getNumVertices() const;

private:
    struct GridVertex {
        float position[3];
    };

    Program m_program;
    int m_numVertices;
    std::shared_ptr<VertexBuffer> m_buffer;
    std::shared_ptr<VertexFormat> m_format;
};


#endif //OPENGL_RENDERER_GRID_H
