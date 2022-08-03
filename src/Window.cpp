#include "Window.h"

int Window::m_numWindows = 0;

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height) {
    if (m_numWindows == 0) {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error("Failed to load SDL.");
        }
    }
    m_numWindows += 1;

    // use OpenGL 4.6 core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // create the SDL window with OpenGL enable
    m_window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            m_width, m_height,
            SDL_WINDOW_OPENGL // | SDL_WINDOW_RESIZABLE
    );

    if (m_window == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }

    // create OpenGL context for window with vsync
    m_context = SDL_GL_CreateContext(m_window);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to load OpenGL.");
    }

    if (!GLEW_ARB_clip_control) {
        throw std::runtime_error("Failed to load required OpenGL clip control extension.");
    }
}

Window::~Window() {
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    m_numWindows -= 1;
    if (m_numWindows == 0) {
        SDL_Quit();
    }
}
