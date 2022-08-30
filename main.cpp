#include "src/engine/Camera3D.h"
#include "src/Grid.h"
#include "src/ObjLoader.h"
#include "src/Window.h"
#include "src/rhi/RHI.h"
#include "src/engine/ShaderLoader.h"
#include "src/ui/Renderer.h"
#include "src/ui/Component.h"

int main(int argc, char* argv[]) {
    int kWidth = 1280, kHeight = 720;
    Window window(kWidth, kHeight, "OpenGL Test");

    RHI& rhi = RHI::current();
    std::unique_ptr<ui::Component> root;

    //std::shared_ptr<Texture2D> texture = Texture2D::fromFile("../assets/lion.png");

    ObjLoader loader;
    loader.load("../assets/flat-monkey.obj");

    struct Vertex {
        float position[3];
        float texture[2];
        float normal[3];
    };

    std::vector<Vertex> vertices;
    for (ObjLoader::Vertex objVertex: loader.getVertices()) {
        vertices.push_back(Vertex{
            {objVertex.position.x, objVertex.position.y, objVertex.position.z},
            {objVertex.texCoord.u, objVertex.texCoord.v},
            {objVertex.normal.x,   objVertex.normal.y,   objVertex.normal.z},
        });
    }

    std::unique_ptr<Buffer> vertexBuffer = rhi.createBuffer(vertices.size() * sizeof(Vertex), sizeof(Vertex));
    std::memcpy(vertexBuffer->map(), vertices.data(), vertexBuffer->size());
    vertexBuffer->unmap();

    std::unique_ptr<Buffer> indexBuffer = rhi.createBuffer(loader.getIndices().size() * sizeof(uint32_t),
                                                            sizeof(uint32_t));
    std::memcpy(indexBuffer->map(), loader.getIndices().data(), indexBuffer->size());
    indexBuffer->unmap();

    std::vector<VertexBinding> bindings = {
        VertexBinding(0, vertexBuffer->stride(), {
            VertexAttribute(0, Format::RGB32F, offsetof(Vertex, position)),
            VertexAttribute(1, Format::RG32F, offsetof(Vertex, texture)),
            VertexAttribute(2, Format::RGB32F, offsetof(Vertex, normal)),
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

    std::shared_ptr<Camera3D> orthographicCamera = Camera3D::makeOrthographic(8.0f, 4.5f, 0.1f, 100.0f);
    std::shared_ptr<Camera3D> perspectiveCamera = Camera3D::makePerspective(45.0, 16.0f / 9.0f, 0.1f, 100.0f);
    std::shared_ptr<Camera3D> camera = perspectiveCamera;

    Grid grid(5.0f, 1.0f);

    float angle = 45.0f;
    float vertAngle = 45.0f;
    float mag = 7.0f;
    float horizMag = mag * std::cos(glm::radians(vertAngle));
    camera->moveTo(Vector<float, 3>{
        horizMag * std::cos(glm::radians(angle)),
        mag * std::sin(glm::radians(vertAngle)),
        horizMag * std::sin(glm::radians(angle))
    });
    camera->lookAt({0.0f, 0.0f, 0.0f}, true);

    std::unique_ptr<Texture2D> colorAttachment = rhi.createTexture2D(Format::RGBA8, kWidth, kHeight);
    std::unique_ptr<Texture2D> depthAttachment = rhi.createTexture2D(Format::D32F, kWidth, kHeight);
    std::unique_ptr<Framebuffer> framebuffer = rhi.createFramebufferBuilder()
        ->setDimensions(kWidth, kHeight)
        ->setColorAttachment(std::move(colorAttachment))
        ->setDepthAttachment(std::move(depthAttachment))
        ->build();

    bool middleDown = false;
    while (!window.shouldClose()) {
        using namespace ui;

        // process pending events
        for (auto event : window.poll()) {
            //root->propagateEvent(event);
            switch (event.type) {
                case EventType::MouseMove:
                    if (middleDown) {
                        angle += event.mouseEvent.deltaX / 2;
                        vertAngle += event.mouseEvent.deltaY / 2;
                        if (vertAngle > 89.0f) vertAngle = 89.0f;
                        else if (vertAngle < -89.0f) vertAngle = -89.0f;
                        horizMag = mag * std::cos(glm::radians(vertAngle));
                        camera->moveTo({
                                           horizMag * std::cos(glm::radians(angle)),
                                           mag * std::sin(glm::radians(vertAngle)),
                                           horizMag * std::sin(glm::radians(angle))
                                       });
                        camera->lookAt({0.0f, 0.0f, 0.0f}, true);
                    }
                    break;
                case EventType::MouseDown:
                    if (event.mouseEvent.button == MouseButton::Middle) {
                        middleDown = true;
                    }
                    break;
                case EventType::MouseUp:
                    if (event.mouseEvent.button == MouseButton::Middle) {
                        middleDown = false;
                    }
                    break;
                case EventType::KeyDown:
                    std::cout << "pressed a key" << std::endl;
                    break;
                case EventType::KeyUp:
                    break;
                case EventType::Wheel:
                    mag -= event.wheelEvent.y / 2;
                    if (mag < 0.1f) mag = 0.1f;
                    horizMag = mag * std::cos(glm::radians(vertAngle));
                    camera->moveTo({
                                       horizMag * std::cos(glm::radians(angle)),
                                       mag * std::sin(glm::radians(vertAngle)),
                                       horizMag * std::sin(glm::radians(angle))
                                   });
                    camera->lookAt({0.0f, 0.0f, 0.0f}, true);
                    break;
            }
        }

        // handle game logic
        // root->update();

        // render pass 1
        rhi.bindFramebuffer(*framebuffer);

        rhi.clearAttachments(0.12f, 0.12f, 0.12f, 1.0f, 1.0f);
        rhi.setViewport(0, 0, kWidth, kHeight);

        UniformBlock uniforms({
                                  MatrixUniform<4, 4>::make(camera->viewProjectionMatrix()),
                                  VectorUniform<float, 3>::make(camera->position())
                              });

        rhi.bindPipeline(grid.pipeline());
        rhi.bindUniforms(uniforms);
        rhi.bindVertexBuffer(grid.vertexBuffer(), 0);
        rhi.draw(grid.numVertices(), 0);

        rhi.bindPipeline(*pipeline);
        rhi.bindUniforms(uniforms);
        rhi.bindVertexBuffer(*vertexBuffer, 0);
        rhi.bindIndexBuffer(*indexBuffer);
        rhi.drawIndexed(indexBuffer->size() / indexBuffer->stride(), 0, 0);

        // render pass 2
        rhi.bindDefaultFramebuffer();

        rhi.clearAttachments(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        rhi.setViewport(0, 0, kWidth, kHeight);

        ui::Renderer renderer(1280.0f, 720.0f);

        ui::RenderList list;

        //root->draw(list);

        list.submitRect(ui::RectInfo{
            .position{0.0f, 0.0f, +0.1f},
            .size{1280.0f, 720.0f},
            .color{0.3f, 0.3f, 0.3f}
        });

        list.submitImage(ui::ImageInfo{
            .position{320.0f, 180.0f, +0.0f},
            .size{640.0f, 360.0f},
            .texture2D = framebuffer->colorAttachment()
        });

        renderer.render(*framebuffer, list);

        window.swap();
    }

    return 0;
}
