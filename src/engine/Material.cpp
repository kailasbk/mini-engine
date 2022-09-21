#include "Material.h"
#include "StaticMeshLoader.h"
#include "ShaderLoader.h"

std::shared_ptr<Material> Material::createDefault() {
    RHI& rhi = RHI::current();

    std::vector<VertexBinding> bindings = {
        VertexBinding(0, sizeof(StaticMeshLoader::Vertex), {
            VertexAttribute(0, Format::RGB32F, offsetof(StaticMeshLoader::Vertex, position)),
            VertexAttribute(1, Format::RG32F, offsetof(StaticMeshLoader::Vertex, texture)),
            VertexAttribute(2, Format::RGB32F, offsetof(StaticMeshLoader::Vertex, normal)),
        })
    };

    VertexLayout layout(std::move(bindings));

    std::unique_ptr<Shader> vertShader = shaderFromFile("../shaders/shader.vert");
    std::unique_ptr<Shader> fragShader = shaderFromFile("../shaders/shader.frag");

    std::unique_ptr<Pipeline> pipeline = rhi.createPipelineBuilder()
        ->setTopology(Topology::Triangles)
        ->setVertexLayout(layout)
        ->setVertexShader(*vertShader)
        ->setFragmentShader(*fragShader)
        ->build();

    return std::make_shared<Material>(std::move(pipeline));
}
