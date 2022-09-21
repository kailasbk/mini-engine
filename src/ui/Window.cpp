#include "Window.h"
#include "../rhi/RHI.h"

namespace ui {

    int Window::m_numWindows = 0;

    Window::Window(int width, int height, const std::string& title)
        : m_width(width), m_height(height), m_shouldClose(false) {
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

        // create the render api
        RHI::create();
    }

    Window::~Window() {
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
        m_numWindows -= 1;
        if (m_numWindows == 0) {
            SDL_Quit();
        }
    }

    Key fromSDLScancode(SDL_Scancode scancode) {
        switch (scancode) {
            case SDL_SCANCODE_A:
                return Key::A;
            case SDL_SCANCODE_B:
                return Key::B;
            case SDL_SCANCODE_C:
                return Key::C;
            case SDL_SCANCODE_D:
                return Key::D;
            case SDL_SCANCODE_E:
                return Key::E;
            case SDL_SCANCODE_F:
                return Key::F;
            case SDL_SCANCODE_G:
                return Key::G;
            case SDL_SCANCODE_H:
                return Key::H;
            case SDL_SCANCODE_I:
                return Key::I;
            case SDL_SCANCODE_J:
                return Key::J;
            case SDL_SCANCODE_K:
                return Key::K;
            case SDL_SCANCODE_L:
                return Key::L;
            case SDL_SCANCODE_M:
                return Key::M;
            case SDL_SCANCODE_N:
                return Key::N;
            case SDL_SCANCODE_O:
                return Key::O;
            case SDL_SCANCODE_P:
                return Key::P;
            case SDL_SCANCODE_Q:
                return Key::Q;
            case SDL_SCANCODE_R:
                return Key::R;
            case SDL_SCANCODE_S:
                return Key::S;
            case SDL_SCANCODE_T:
                return Key::T;
            case SDL_SCANCODE_U:
                return Key::U;
            case SDL_SCANCODE_V:
                return Key::V;
            case SDL_SCANCODE_W:
                return Key::W;
            case SDL_SCANCODE_X:
                return Key::X;
            case SDL_SCANCODE_Y:
                return Key::Y;
            case SDL_SCANCODE_Z:
                return Key::Z;
            case SDL_SCANCODE_1:
                return Key::One;
            case SDL_SCANCODE_2:
                return Key::Two;
            case SDL_SCANCODE_3:
                return Key::Three;
            case SDL_SCANCODE_4:
                return Key::Four;
            case SDL_SCANCODE_5:
                return Key::Five;
            case SDL_SCANCODE_6:
                return Key::Six;
            case SDL_SCANCODE_7:
                return Key::Seven;
            case SDL_SCANCODE_8:
                return Key::Eight;
            case SDL_SCANCODE_9:
                return Key::Nine;
            case SDL_SCANCODE_0:
                return Key::Zero;
            case SDL_SCANCODE_RETURN:
                return Key::Return;
            case SDL_SCANCODE_ESCAPE:
                return Key::Escape;
            case SDL_SCANCODE_BACKSPACE:
                return Key::Backspace;
            case SDL_SCANCODE_TAB:
                return Key::Tab;
            case SDL_SCANCODE_SPACE:
                return Key::Space;
            case SDL_SCANCODE_MINUS:
                return Key::Minus;
            case SDL_SCANCODE_EQUALS:
                return Key::Equals;
            case SDL_SCANCODE_LEFTBRACKET:
                return Key::LeftBracket;
            case SDL_SCANCODE_RIGHTBRACKET:
                return Key::RightBracket;
            case SDL_SCANCODE_BACKSLASH:
                return Key::Backslash;
            case SDL_SCANCODE_SEMICOLON:
                return Key::Semicolon;
            case SDL_SCANCODE_APOSTROPHE:
                return Key::Apostrophe;
            case SDL_SCANCODE_GRAVE:
                return Key::Grave;
            case SDL_SCANCODE_COMMA:
                return Key::Comma;
            case SDL_SCANCODE_PERIOD:
                return Key::Period;
            case SDL_SCANCODE_SLASH:
                return Key::Slash;
            case SDL_SCANCODE_CAPSLOCK:
                return Key::CapsLock;
            case SDL_SCANCODE_F1:
                return Key::F1;
            case SDL_SCANCODE_F2:
                return Key::F2;
            case SDL_SCANCODE_F3:
                return Key::F3;
            case SDL_SCANCODE_F4:
                return Key::F4;
            case SDL_SCANCODE_F5:
                return Key::F5;
            case SDL_SCANCODE_F6:
                return Key::F6;
            case SDL_SCANCODE_F7:
                return Key::F7;
            case SDL_SCANCODE_F8:
                return Key::F8;
            case SDL_SCANCODE_F9:
                return Key::F9;
            case SDL_SCANCODE_F10:
                return Key::F10;
            case SDL_SCANCODE_F11:
                return Key::F11;
            case SDL_SCANCODE_F12:
                return Key::F12;
            case SDL_SCANCODE_RIGHT:
                return Key::Right;
            case SDL_SCANCODE_LEFT:
                return Key::Left;
            case SDL_SCANCODE_DOWN:
                return Key::Down;
            case SDL_SCANCODE_UP:
                return Key::Up;
            case SDL_SCANCODE_LCTRL:
            case SDL_SCANCODE_LSHIFT:
            case SDL_SCANCODE_LALT:
            case SDL_SCANCODE_LGUI:
            case SDL_SCANCODE_RCTRL:
            case SDL_SCANCODE_RSHIFT:
            case SDL_SCANCODE_RALT:
            case SDL_SCANCODE_RGUI:
            default:
                return Key::Unsupported;
        }
    }

    MouseButton fromSDLMouseButton(Uint8 sdlButton) {
        switch (sdlButton) {
            case SDL_BUTTON_LEFT:
                return MouseButton::Left;
            case SDL_BUTTON_MIDDLE:
                return MouseButton::Middle;
            case SDL_BUTTON_RIGHT:
                return MouseButton::Right;
            default:
                return MouseButton::Left;
        }
    }

    std::vector<Event> Window::poll() {
        std::vector<Event> events;

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_WINDOWEVENT:
                    if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
                        m_width = sdlEvent.window.data1;
                        m_height = sdlEvent.window.data2;
                    } else if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
                        m_shouldClose = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    events.push_back(Event{
                        .type = EventType::KeyDown,
                        .keyEvent = KeyEvent{
                            .key = fromSDLScancode(sdlEvent.key.keysym.scancode)
                        },
                    });
                    break;
                case SDL_KEYUP:
                    events.push_back(Event{
                        .type = EventType::KeyUp,
                        .keyEvent = KeyEvent{
                            .key = fromSDLScancode(sdlEvent.key.keysym.scancode)
                        },
                    });
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    events.push_back(Event{
                        .type = EventType::MouseDown,
                        .mouseEvent = MouseEvent{
                            .x = (float)sdlEvent.button.x,
                            .y = (float)sdlEvent.button.y,
                            .deltaX = 0,
                            .deltaY = 0,
                            .button = fromSDLMouseButton(sdlEvent.button.button),
                        }
                    });
                    break;
                case SDL_MOUSEBUTTONUP:
                    events.push_back(Event{
                        .type = EventType::MouseUp,
                        .mouseEvent = MouseEvent{
                            .x = (float)sdlEvent.button.x,
                            .y = (float)sdlEvent.button.y,
                            .deltaX = 0,
                            .deltaY = 0,
                            .button = fromSDLMouseButton(sdlEvent.button.button),
                        }
                    });
                    break;
                case SDL_MOUSEMOTION:
                    events.push_back(Event{
                        .type = EventType::MouseMove,
                        .mouseEvent = MouseEvent{
                            .x = (float)sdlEvent.motion.x,
                            .y = (float)sdlEvent.motion.y,
                            .deltaX = (float)sdlEvent.motion.xrel,
                            .deltaY = (float)sdlEvent.motion.yrel,
                            .button = MouseButton::None,
                        }
                    });
                    break;
                case SDL_MOUSEWHEEL:
                    events.push_back(Event{
                        .type = EventType::Wheel,
                        .wheelEvent = WheelEvent{
                            .y = sdlEvent.wheel.preciseY,
                        }
                    });
                    break;
            }
        }

        return events;
    }

    void Window::swap() const {
        SDL_GL_SwapWindow(m_window);
    }

} // ui