#include "StaticMeshLoader.h"

StaticMesh StaticMeshLoader::load(const std::string& filename) {
    RHI& rhi = RHI::current();
    m_objLoader.load(filename);

    std::vector<Vertex> vertices;
    for (ObjLoader::Vertex objVertex: m_objLoader.getVertices()) {
        vertices.push_back(Vertex{
            {objVertex.position.x, objVertex.position.y, objVertex.position.z},
            {objVertex.texCoord.u, objVertex.texCoord.v},
            {objVertex.normal.x,   objVertex.normal.y,   objVertex.normal.z},
        });
    }

    std::unique_ptr<Buffer> vertexBuffer = rhi.createBuffer(vertices.size() * sizeof(Vertex), sizeof(Vertex));
    std::memcpy(vertexBuffer->map(), vertices.data(), vertexBuffer->size());
    vertexBuffer->unmap();

    std::unique_ptr<Buffer> indexBuffer = rhi.createBuffer(m_objLoader.getIndices().size() * sizeof(uint32_t),
                                                           sizeof(uint32_t));
    std::memcpy(indexBuffer->map(), m_objLoader.getIndices().data(), indexBuffer->size());
    indexBuffer->unmap();

    return StaticMesh{
        .vertexBuffer = std::move(vertexBuffer),
        .indexBuffer = std::move(indexBuffer),
        .material = m_defaultMaterial,
    };
}
