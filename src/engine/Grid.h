#ifndef OPENGL_RENDERER_GRID_H
#define OPENGL_RENDERER_GRID_H

#include "../rhi/RHI.h"
#include "StaticMesh.h"

class Grid {
public:
    /**
     * Constructs a 2*size by 2*getNumIndices square grid with tiles of tileSize by tileSize.
     *
     * @param size the width of the grid
     * @param tileSize the width of the grid tiles
     */
    static StaticMesh make(float size, float tileSize);
};


#endif //OPENGL_RENDERER_GRID_H
