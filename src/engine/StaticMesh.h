#ifndef OPENGL_RENDERER_STATICMESH_H
#define OPENGL_RENDERER_STATICMESH_H

#include "../rhi/RHI.h"
#include "Material.h"

/**
 * A static mesh which has a vertex buffer, index buffer, and a single material that dictates
 * how to draw the mesh. The vertices in the vertex buffer are of unspecified format.
 * The mesh is considered renderable if it have at least a valid vertex buffer and material.
 */
struct StaticMesh {
    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indexBuffer;
    std::shared_ptr<Material> material;

    /**
     * @returns whether the mesh has a vertex buffer and material, and is thus renderable
     */
    bool isRenderable() const {
        return vertexBuffer != nullptr && material != nullptr;
    }

    /**
     * @returns whether the mesh has an index buffer
     */
    bool isIndexed() const {
        return indexBuffer != nullptr;
    }
};


#endif //OPENGL_RENDERER_STATICMESH_H
