#include "Grid.h"

#include "util/ShaderLoader.h"

Grid::Grid(float width, float tileSize) {
    RenderAPI* api = defaultRenderAPI();

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

    m_numVertices = lines.size();
    m_buffer = api->createBuffer(m_numVertices * sizeof(GridVertex), sizeof(GridVertex));
    std::memcpy(m_buffer->map(), lines.data(), m_buffer->size());

    VertexLayout vertexLayout({
        VertexBinding(0, m_buffer->size(), {
            VertexAttribute(0, Format::RGB32F, offsetof(GridVertex, position))
        })
    });

    std::unique_ptr<Shader> vertShader = shaderFromFile("../shaders/grid.vert");
    std::unique_ptr<Shader> fragShader = shaderFromFile("../shaders/grid.frag");

    m_pipeline = api->createPipelineBuilder()
            ->setTopology(Topology::Lines)
            ->setVertexLayout(vertexLayout)
            ->setVertexShader(*vertShader)
            ->setFragmentShader(*fragShader)
            ->build();
}
