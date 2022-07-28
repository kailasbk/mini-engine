#include "src/IndexBuffer.h"
#include "src/VertexBuffer.h"
#include "src/VertexFormat.h"
#include "src/Shader.h"
#include "src/Program.h"
#include "src/Math.h"
#include "src/Camera3D.h"
#include "src/Grid.h"
#include "src/ObjLoader.h"
#include "src/Texture2D.h"
#include "src/Framebuffer.h"
#include "src/Window.h"

int main(int argc, char* argv[]) {
    int kWidth = 1280, kHeight = 720;
    Window window(kWidth, kHeight, "OpenGL Test");

    std::shared_ptr<Texture2D> texture = Texture2D::fromFile("../assets/lion.png");

    ObjLoader loader;
    loader.load("../assets/flat-monkey.obj");

    struct Vertex {
        float position[3];
        float texture[2];
        float normal[3];
    };

    std::vector<Vertex> vertices;
    for (ObjLoader::Vertex objVertex : loader.getVertices()) {
        vertices.push_back({
                {objVertex.position.x, objVertex.position.y, objVertex.position.z},
                {objVertex.texCoord.u, objVertex.texCoord.v},
                {objVertex.normal.x, objVertex.normal.y, objVertex.normal.z},
        });
    }
    VertexBuffer vertexBuffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), sizeof(Vertex));

    IndexBuffer indexBuffer = IndexBuffer(loader.getIndices().data(), loader.getIndices().size(), sizeof(unsigned int));

    /*
    std::vector<Vertex> vertices = {
            {{-1.0f, +2.0f, +1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f, +0.0f, +1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{+1.0f, +0.0f, +1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{+1.0f, +2.0f, +1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
    };

    VertexBuffer vertexBuffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), sizeof(Vertex));

    std::vector<unsigned int> indices {
            0, 1, 2,
            2, 3, 0,
    };
    IndexBuffer indexBuffer = IndexBuffer(indices.data(), indices.size(), sizeof(unsigned int));
    */

    std::vector<VertexFormat::Attribute> attributes = {
            {0, 0, math::Float, 3, true, false, (int)offsetof(Vertex, position)},
            {1, 0, math::Float, 2, true, false, (int)offsetof(Vertex, texture)},
            {2, 0, math::Float, 3, true, false, (int)offsetof(Vertex, normal)},
    };
    VertexFormat format = VertexFormat(attributes);

    std::shared_ptr<Shader> vertShader = Shader::fromFile("../shaders/shader.vert");
    std::shared_ptr<Shader> fragShader = Shader::fromFile("../shaders/shader.frag");

    Program program;
    program.addShader(vertShader, Shader::Type::Vertex);
    program.addShader(fragShader, Shader::Type::Fragment);
    program.link();

    std::shared_ptr<Camera3D> orthographicCamera = Camera3D::makeOrthographic(8.0f, 4.5f, 0.1f, 100.0f);
    std::shared_ptr<Camera3D> perspectiveCamera = Camera3D::makePerspective(45.0, 16.0f / 9.0f, 0.1f, 100.0f);
    std::shared_ptr<Camera3D> camera = perspectiveCamera;

    Grid grid(5.0f, 1.0f);

    float angle = 45.0f;
    float vertAngle = 45.0f;
    float mag = 7.0f;
    float horizMag = mag * std::cos(glm::radians(vertAngle));
    camera->moveTo(glm::vec3(
            horizMag * std::cos(glm::radians(angle)),
            mag * std::sin(glm::radians(vertAngle)),
            horizMag * std::sin(glm::radians(angle))
    ));
    camera->lookAt(glm::vec3(0.0f), true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    /*
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    */

    auto colorAttachment = std::make_shared<Texture2D>(kWidth / 2, kHeight / 2, 4);
    Framebuffer framebuffer(kWidth / 2, kHeight / 2);
    framebuffer.addColorAttachment(colorAttachment);
    framebuffer.addDepthAttachment();

    auto interColorAttachment = std::make_shared<Texture2D>(kWidth / 2, kHeight / 2);
    Framebuffer interFramebuffer(kWidth / 2, kHeight / 2);
    interFramebuffer.addColorAttachment(interColorAttachment);

    struct QuadVertex {
        float position[3];
        float texture[2];
    };

    std::vector<QuadVertex> quadVertices = {
            {{-0.5f, +0.5f, +0.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f, +0.0f}, {0.0f, 0.0f}},
            {{+0.5f, -0.5f, +0.0f}, {1.0f, 0.0f}},
            {{+0.5f, +0.5f, +0.0f}, {1.0f, 1.0f}},
    };
    VertexBuffer quadVertexBuffer = VertexBuffer(quadVertices.data(), quadVertices.size() * sizeof(QuadVertex), sizeof(QuadVertex));

    std::vector<VertexFormat::Attribute> quadAttributes = {
            {0, 0, math::Float, 3, true, false, (int)offsetof(QuadVertex, position)},
            {1, 0, math::Float, 2, true, false, (int)offsetof(QuadVertex, texture)},
    };
    VertexFormat quadFormat = VertexFormat(quadAttributes);

    std::shared_ptr<Shader> quadVertShader = Shader::fromFile("../shaders/ui.vert");
    std::shared_ptr<Shader> quadFragShader = Shader::fromFile("../shaders/ui.frag");

    Program quadProgram;
    quadProgram.addShader(quadVertShader, Shader::Type::Vertex);
    quadProgram.addShader(quadFragShader, Shader::Type::Fragment);
    quadProgram.link();

    bool close = false;
    bool middleDown = false;
    while (!close) {
        // process pending events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    std::cout << "pressed a key: " << event.key.keysym.sym << std::endl;
                    break;
                case SDL_WINDOWEVENT:
                    /*
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        glViewport(0, 0, event.window.data1, event.window.data2);
                    }
                    */
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                        close = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_MIDDLE) {
                        middleDown = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_MIDDLE) {
                        middleDown = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (middleDown) {
                        angle += (float)event.motion.xrel / 2;
                        vertAngle += (float)event.motion.yrel / 2;
                        if (vertAngle > 89.0f) vertAngle = 89.0f;
                        else if (vertAngle < -89.0f) vertAngle = -89.0f;
                        horizMag = mag * std::cos(glm::radians(vertAngle));
                        camera->moveTo(glm::vec3(
                                horizMag * std::cos(glm::radians(angle)),
                                mag * std::sin(glm::radians(vertAngle)),
                                horizMag * std::sin(glm::radians(angle))
                        ));
                        camera->lookAt(glm::vec3(0.0f), true);
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    mag -= event.wheel.preciseY / 2;
                    if (mag < 0.1f) mag = 0.1f;
                    horizMag = mag * std::cos(glm::radians(vertAngle));
                    camera->moveTo(glm::vec3(
                            horizMag * std::cos(glm::radians(angle)),
                            mag * std::sin(glm::radians(vertAngle)),
                            horizMag * std::sin(glm::radians(angle))
                    ));
                    camera->lookAt(glm::vec3(0.0f), true);
                    break;
            }
        }

        // handle game logic

        // draw the frame
        framebuffer.bind();
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, kWidth / 2, kHeight / 2);

        grid.getProgram().bind();
        grid.getProgram().setMat4("MVP", camera->getViewProjectionMatrix());
        grid.getVertexFormat().bind();
        grid.getVertexBuffer().bind(0);
        glDrawArrays(GL_LINES, 0, grid.getNumVertices());

        program.bind();
        program.setMat4("MVP", camera->getViewProjectionMatrix());
        program.setVec3("LightPosition", camera->getPosition());
        format.bind();
        vertexBuffer.bind(0);
        indexBuffer.bind();
        texture->bind(0);
        glDrawElements(GL_TRIANGLES, indexBuffer.getNumIndices(), GL_UNSIGNED_INT, nullptr);
        Framebuffer::unbind();

        interFramebuffer.copyColorAttachments(framebuffer);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, kWidth, kHeight);
        quadProgram.bind();
        quadFormat.bind();
        quadVertexBuffer.bind();
        indexBuffer.bind();
        interColorAttachment->bind(0);
        glDrawElements(GL_TRIANGLES, indexBuffer.getNumIndices(), GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(window.m_window);
    }

    return 0;
}
