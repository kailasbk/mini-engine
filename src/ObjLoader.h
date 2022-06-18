#ifndef OPENGL_RENDERER_OBJLOADER_H
#define OPENGL_RENDERER_OBJLOADER_H

#include <string>
#include <vector>

class ObjLoader {
public:
    struct Position {
        float x, y, z, w;
    };

    struct Normal {
        float x, y, z;
    };

    struct TexCoord {
        float u, v, w;
    };

    struct Vertex {
        Position position;
        TexCoord texCoord;
        Normal normal;
    };

    /**
     * Constructs a mesh loader.
     */
    ObjLoader();

    /**
     * Loads a .obj mesh from the file with the given string.
     *
     * @param string the string of the file
     */
    void load(const std::string& string);

    const std::vector<Vertex>& getVertices();
    const std::vector<unsigned int>& getIndices();
private:
    unsigned int m_numVertices;
    unsigned int m_numIndices;

    std::vector<Position> m_positions;
    std::vector<Normal> m_normals;
    std::vector<TexCoord> m_texCoords;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};


#endif //OPENGL_RENDERER_OBJLOADER_H
