#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "src/IndexBuffer.h"
#include "src/VertexBuffer.h"
#include "src/VertexFormat.h"
#include "src/Shader.h"
#include "src/Program.h"
#include "src/Math.h"
#include "src/Camera.h"
#include "src/Grid.h"
#include "src/ObjLoader.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to init SDL." << std::endl;
    }

    // use OpenGL 4.6 core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // turn on 4x MSAA
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    const int kWidth = 1280, kHeight = 720;
    SDL_Window* window = SDL_CreateWindow(
            "OpenGL Renderer",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            kWidth, kHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW." << std::endl;
    }

    // set the openGL viewport to window getNumIndices
    glViewport(0, 0, kWidth, kHeight);

    ObjLoader loader;
    loader.load("../assets/flat-monkey.obj");

    struct Vertex {
        float position[3];
        float texture[2];
        float normal[3];
    };

    // vertices for the screen quad
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

    std::vector<VertexFormat::Attribute> attributes = {
            {0, 0, math::Float, 3, true, false, offsetof(Vertex, position)},
            {1, 0, math::Float, 2, true, false, offsetof(Vertex, texture)},
            {2, 0, math::Float, 3, true, false, offsetof(Vertex, normal)},
    };
    VertexFormat format = VertexFormat(attributes);

    std::shared_ptr<Shader> vertShader = Shader::fromFile("../shaders/shader.vert");
    std::shared_ptr<Shader> fragShader = Shader::fromFile("../shaders/shader.frag");

    Program program;
    program.addShader(vertShader, Shader::Type::Vertex);
    program.addShader(fragShader, Shader::Type::Fragment);
    program.link();

    std::shared_ptr<Camera> orthographicCamera = Camera::makeOrthographic(8.0f, 4.5f, 0.1f, 100.0f);
    std::shared_ptr<Camera> perspectiveCamera = Camera::makePerspective(45.0, 16.0f / 9.0f, 0.1f, 100.0f);
    std::shared_ptr<Camera> camera = perspectiveCamera;

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

    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);

    bool close = false;
    bool middleDown = false;
    while (!close) {
        // process pending events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_KEYDOWN:
                    std::cout << "pressed a key: " << event.key.keysym.sym << std::endl;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        glViewport(0, 0, event.window.data1, event.window.data2);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        glDrawElements(GL_TRIANGLES, indexBuffer.getNumIndices(), GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
