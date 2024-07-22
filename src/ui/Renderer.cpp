#include "Renderer.h"

#include "../engine/ShaderLoader.h"

namespace ui {

Renderer::Renderer(float width, float height)
    : width_(width), height_(height)
{
    RHI& rhi = RHI::current();

    // constructs an index buffer for quads
    index_buffer_ = rhi.createBuffer(sizeof(uint32_t) * 6, sizeof(uint32_t));
    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0,
    };
    std::memcpy(index_buffer_->map(), indices.data(), index_buffer_->size());
    index_buffer_->unmap();

    vertex_buffer_ = rhi.createBuffer(sizeof(Vertex) * 4, sizeof(Vertex));

    std::vector<VertexBinding> bindings = {
        VertexBinding(0, vertex_buffer_->stride(), {
            VertexAttribute(0, Format::RGB32F, offsetof(Vertex, position)),
            VertexAttribute(1, Format::RG32F, offsetof(Vertex, texture)),
        })
    };

    VertexLayout layout(std::move(bindings));

    auto vert_shader = shaderFromFile("../shaders/ui-image.vert");
    auto frag_shader = shaderFromFile("../shaders/ui-image.frag");

    image_pipeline_ = rhi.createPipelineBuilder()
        ->setTopology(Topology::Triangles)
        ->setVertexLayout(layout)
        ->setVertexShader(*vert_shader)
        ->setFragmentShader(*frag_shader)
        ->build();

    vert_shader = shaderFromFile("../shaders/ui-rect.vert");
    frag_shader = shaderFromFile("../shaders/ui-rect.frag");

    rect_pipeline_ = rhi.createPipelineBuilder()
        ->setTopology(Topology::Triangles)
        ->setVertexLayout(layout)
        ->setVertexShader(*vert_shader)
        ->setFragmentShader(*frag_shader)
        ->build();
};

void Renderer::set_resolution(float width, float height)
{
    width_ = width;
    height_ = height;
}

void Renderer::render(const RenderList& list)
{
    RHI& rhi = RHI::current();

    rhi.bindDefaultFramebuffer();

    rhi.clearAttachments(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    rhi.setViewport(0, 0, 1290, 730); // TODO: remove magic number here

    // render images then rects then text
    auto descriptor_set = rhi.createDescriptorSet({
        DescriptorSetBinding {
            .binding = 0,
            .type = DescriptorType::Texture2D,
        },
    });

    // bind the image shader pipeline
    rhi.bindPipeline(*image_pipeline_);

    // TODO: modify to squash z coordinates so +z is closer, 0 is farthest
    std::array<Vec4, 4> cols = {
        Vec4{2.0f/width_, 0.0f, 0.0f, 0.0f},
        Vec4{0.0f, -2.0f/height_, 0.0f, 0.0f},
        Vec4{0.0f, 0.0f, 1.0f, 0.0f},
        Vec4{-1.0f, 1.0f, 0.0f, 1.0f},
    };
    Mat4 projection(cols);
    Vec4 color;

    UniformBlock uniform_block({
        MatrixUniform<4, 4>::make(projection),
        VectorUniform<float, 4>::make(color),
    });
    rhi.bindUniforms(uniform_block);

    rhi.bindIndexBuffer(*index_buffer_);
    rhi.bindVertexBuffer(*vertex_buffer_, 0);

    for (auto& image : list.images()) {
        float left = image.position.x;
        float right = image.position.x + image.size.width;
        float top = image.position.y;
        float bottom = image.position.y + image.size.height;
        float z = image.position.z;

        // create the vertices for the image
        std::array<Vertex, 4> vertices = {
            Vertex{{left, top, z}, {0.0f, 1.0f}}, // top left
            Vertex{{left, bottom, z}, {0.0f, 0.0f}}, // bottom left
            Vertex{{right, bottom, z}, {1.0f, 0.0f}}, // bottom right
            Vertex{{right, top, z}, {1.0f, 1.0f}}, // top right
        };
        std::memcpy(vertex_buffer_->map(), vertices.data(), vertex_buffer_->size());
        vertex_buffer_->unmap();

        // bind the texture
        descriptor_set->bindTexture2D(0, image.texture2d);
        rhi.bindDescriptorSet(*descriptor_set);

        // draw the image
        rhi.drawIndexed(6, 0, 0);
    }

    // bind the rect shader pipeline
    rhi.bindPipeline(*rect_pipeline_);

    for (auto& rect : list.rects()) {
        float left = rect.position.x;
        float right = rect.position.x + rect.size.width;
        float top = rect.position.y;
        float bottom = rect.position.y + rect.size.height;
        float z = rect.position.z;

        // create the vertices for the rect
        std::array<Vertex, 4> vertices = {
            Vertex{{left, top, z}, {}}, // top left
            Vertex{{left, bottom, z}, {}}, // bottom left
            Vertex{{right, bottom, z}, {}}, // bottom right
            Vertex{{right, top, z}, {}}, // top right
        };
        std::memcpy(vertex_buffer_->map(), vertices.data(), vertex_buffer_->size());
        vertex_buffer_->unmap();

        // update the color uniform
        color.x = rect.color.r;
        color.y = rect.color.g;
        color.z = rect.color.b;
        color.w = rect.color.a;
        rhi.bindUniforms(uniform_block);

        // draw the rect
        rhi.drawIndexed(6, 0, 0);
    }

    for (auto text : list.texts()) {
        // TODO: need to find a way to make an atlas, and then index into that texture
        // should start with the simple letters and numbers
    }

    vertex_buffer_->unmap();
}

} // ui
