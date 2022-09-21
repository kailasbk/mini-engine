#ifndef OPENGL_RENDERER_STATICMESHLOADER_H
#define OPENGL_RENDERER_STATICMESHLOADER_H

#include "ObjLoader.h"
#include "StaticMesh.h"

class StaticMeshLoader {
public:
    /**
     * The vertex format used by the loader.
     */
    struct Vertex {
        float position[3];
        float texture[2];
        float normal[3];
    };

    /**
     * Constructs a static mesh loader with a given default material.
     *
     * @param defaultMaterial the default material to apply to loaded meshes, must not be nullptr
     */
    explicit StaticMeshLoader(std::shared_ptr<Material> defaultMaterial)
        : m_defaultMaterial(std::move(defaultMaterial)) {
        if (m_defaultMaterial == nullptr) {
            throw std::invalid_argument("StaticMeshLoader requires a default material.");
        }
    }

    /**
     * Loads a static mesh from the given file. The mesh is assigned a default material
     * so that it is renderable immediately. Meshes returned by this function contain vertices
     * that follow the format given by the Vertex struct.
     *
     * @see Vertex
     * @param filename the name of the file
     * @returns a static mesh wit
     */
    StaticMesh load(const std::string& filename);

private:
    std::shared_ptr<Material> m_defaultMaterial;
    ObjLoader m_objLoader;
};


#endif //OPENGL_RENDERER_STATICMESHLOADER_H
