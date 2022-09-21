#include "Grid.h"

#include "ShaderLoader.h"

StaticMesh Grid::make(float width, float tileSize) {
    RHI& rhi = RHI::current();

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

    uint32_t m_numVertices = lines.size();
    auto buffer = rhi.createBuffer(m_numVertices * sizeof(GridVertex), sizeof(GridVertex));
    std::memcpy(buffer->map(), lines.data(), buffer->size());

    std::vector<VertexBinding> bindings = {
        VertexBinding(0, buffer->size(), {
            VertexAttribute(0, Format::RGB32F, offsetof(GridVertex, position))
        })
    };

    VertexLayout vertexLayout(std::move(bindings));

    std::unique_ptr<Shader> vertShader = shaderFromFile("../shaders/grid.vert");
    std::unique_ptr<Shader> fragShader = shaderFromFile("../shaders/grid.frag");

    auto pipeline = rhi.createPipelineBuilder()
        ->setTopology(Topology::Lines)
        ->setVertexLayout(vertexLayout)
        ->setVertexShader(*vertShader)
        ->setFragmentShader(*fragShader)
        ->build();

    return StaticMesh{
        .vertexBuffer = std::move(buffer),
        .indexBuffer = nullptr,
        .material = std::make_shared<Material>(std::move(pipeline))
    };
}
