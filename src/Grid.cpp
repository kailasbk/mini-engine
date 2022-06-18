#include "Grid.h"

#include <cmath>
#include <iostream>

Grid::Grid(float width, float tileSize) {
    struct GridVertex {
        float position[3];
    };

    // number of outer lines in each direction
    int numLines = std::floor(width / tileSize);

    std::vector<GridVertex> lines;
    lines.reserve(2 * 2 * (numLines + 1));

    float posBound = width;
    float negBound = -width;

    for (int x = -numLines; x <= numLines; x++) {
        lines.push_back({{(float)x * tileSize, 0.0f, posBound}});
        lines.push_back({{(float)x * tileSize, 0.0f, negBound}});
    }

    for (int z = -numLines; z <= numLines; z++) {
        lines.push_back({{posBound, 0.0f, (float)z * tileSize}});
        lines.push_back({{negBound, 0.0f, (float)z * tileSize}});
    }

    m_numVertices = (int)lines.size();
    m_buffer = std::make_shared<VertexBuffer>(lines.data(), m_numVertices * sizeof(GridVertex), sizeof(GridVertex));
    std::vector<VertexFormat::Attribute> gridAttributes = {
            {0, 0, math::Float, 3, true, false, offsetof(GridVertex, position)},
    };
    m_format = std::make_shared<VertexFormat>(gridAttributes);

    std::shared_ptr<Shader> gridVertShader = Shader::fromFile("../shaders/grid.vert");
    std::shared_ptr<Shader> gridFragShader = Shader::fromFile("../shaders/grid.frag");

    m_program.addShader(gridVertShader, Shader::Type::Vertex);
    m_program.addShader(gridFragShader, Shader::Type::Fragment);
    m_program.link();
}

const VertexBuffer& Grid::getVertexBuffer() {
    return *m_buffer;
}

const VertexFormat& Grid::getVertexFormat() {
    return *m_format;
}

const Program& Grid::getProgram() {
    return m_program;
}

int Grid::getNumVertices() const {
    return m_numVertices;
}
