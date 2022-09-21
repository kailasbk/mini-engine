#include "Renderer.h"
#include "../engine/ShaderLoader.h"

namespace ui {

    Renderer::Renderer(float width, float height)
        : m_width(width), m_height(height) {
        RHI& rhi = RHI::current();

        // constructs an index buffer for quads
        std::vector<uint32_t> indices {
            0, 1, 2,
            2, 3, 0,
        };

        m_indexBuffer = rhi.createBuffer(sizeof(uint32_t) * 6, sizeof(uint32_t));
        std::memcpy(m_indexBuffer->map(), indices.data(), m_indexBuffer->size());
        m_indexBuffer->unmap();

        m_vertexBuffer = rhi.createBuffer(sizeof(Vertex) * 4, sizeof(Vertex));

        std::vector<VertexBinding> bindings = {
            VertexBinding(0, m_vertexBuffer->stride(), {
                VertexAttribute(0, Format::RGB32F, offsetof(Vertex, position)),
                VertexAttribute(1, Format::RG32F, offsetof(Vertex, texture)),
            })
        };

        VertexLayout layout(std::move(bindings));

        std::unique_ptr<Shader> vertShader = shaderFromFile("../shaders/ui-image.vert");
        std::unique_ptr<Shader> fragShader = shaderFromFile("../shaders/ui-image.frag");

        m_imagePipeline = rhi.createPipelineBuilder()
            ->setTopology(Topology::Triangles)
            ->setVertexLayout(layout)
            ->setVertexShader(*vertShader)
            ->setFragmentShader(*fragShader)
            ->build();

        vertShader = shaderFromFile("../shaders/ui-rect.vert");
        fragShader = shaderFromFile("../shaders/ui-rect.frag");

        m_rectPipeline = rhi.createPipelineBuilder()
            ->setTopology(Topology::Triangles)
            ->setVertexLayout(layout)
            ->setVertexShader(*vertShader)
            ->setFragmentShader(*fragShader)
            ->build();
    };

    void Renderer::setResolution(float width, float height) {
        m_width = width;
        m_height = height;
    }

    void Renderer::render(const RenderList& list) {
        RHI& rhi = RHI::current();

        rhi.bindDefaultFramebuffer();

        rhi.clearAttachments(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        rhi.setViewport(0, 0, 1290, 730); // TODO: remove magic number here

        // render images then rects then text
        auto descriptorSet = rhi.createDescriptorSet({{0, DescriptorType::Texture2D}});

        // bind the image shader pipeline
        rhi.bindPipeline(*m_imagePipeline);

        // TODO: modify to squash z coordinates so +z is closer, 0 is farthest
        std::array<Vec4, 4> cols = {
            Vec4{2.0f/m_width, 0.0f, 0.0f, 0.0f},
            Vec4{0.0f, -2.0f/m_height, 0.0f, 0.0f},
            Vec4{0.0f, 0.0f, 1.0f, 0.0f},
            Vec4{-1.0f, 1.0f, 0.0f, 1.0f},
        };
        Mat4 projection(cols);
        Vec4 color;

        UniformBlock uniformBlock({
            MatrixUniform<4, 4>::make(projection),
            VectorUniform<float, 4>::make(color),
        });
        rhi.bindUniforms(uniformBlock);

        rhi.bindIndexBuffer(*m_indexBuffer);
        rhi.bindVertexBuffer(*m_vertexBuffer, 0);

        for (auto image : list.images()) {
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
            std::memcpy(m_vertexBuffer->map(), vertices.data(), m_vertexBuffer->size());
            m_vertexBuffer->unmap();

            // bind the texture
            descriptorSet->bindTexture2D(0, image.texture2D);
            rhi.bindDescriptorSet(*descriptorSet);

            // draw the image
            rhi.drawIndexed(6, 0, 0);
        }

        // bind the rect shader pipeline
        rhi.bindPipeline(*m_rectPipeline);

        for (auto rect : list.rects()) {
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
            std::memcpy(m_vertexBuffer->map(), vertices.data(), m_vertexBuffer->size());
            m_vertexBuffer->unmap();

            // update the color uniform
            color.x = rect.color.r;
            color.y = rect.color.g;
            color.z = rect.color.b;
            color.w = rect.color.a;
            rhi.bindUniforms(uniformBlock);

            // draw the rect
            rhi.drawIndexed(6, 0, 0);
        }

        for (auto text : list.texts()) {
            // TODO: need to find a way to make an atlas, and then index into that texture
            // should start with the simple letters and numbers
        }

        m_vertexBuffer->unmap();
    }

} // ui