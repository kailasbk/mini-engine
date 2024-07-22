#include "Window.h"

#include "../rhi/RHI.h"

namespace ui {

int Window::num_windows_ = 0;

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), should_close_(false)
{
    if (num_windows_ == 0) {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error("Failed to load SDL.");
        }
    }
    num_windows_ += 1;

    // use OpenGL 4.6 core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // create the SDL window with OpenGL enable
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width_, height_,
        SDL_WINDOW_OPENGL // | SDL_WINDOW_RESIZABLE
    );

    if (window_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }

    // create OpenGL context for window with vsync
    context_ = SDL_GL_CreateContext(window_);
    SDL_GL_SetSwapInterval(1);

    // create the render api
    RHI::create();
}

Window::~Window()
{
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    num_windows_ -= 1;
    if (num_windows_ == 0) {
        SDL_Quit();
    }
}

Key from_sdl_scancode(SDL_Scancode scancode)
{
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

MouseButton from_sdl_mouse_button(Uint8 sdl_button)
{
    switch (sdl_button) {
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

std::vector<Event> Window::poll()
{
    std::vector<Event> events;

    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
        case SDL_WINDOWEVENT:
            if (sdl_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                width_ = sdl_event.window.data1;
                height_ = sdl_event.window.data2;
            } else if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE) {
                should_close_ = true;
            }
            break;
        case SDL_KEYDOWN:
            events.push_back(Event {
                .type = EventType::KeyDown,
                .key_event = KeyEvent {
                    .key = from_sdl_scancode(sdl_event.key.keysym.scancode)
                },
            });
            break;
        case SDL_KEYUP:
            events.push_back(Event {
                .type = EventType::KeyUp,
                .key_event = KeyEvent {
                    .key = from_sdl_scancode(sdl_event.key.keysym.scancode)
                },
            });
            break;
        case SDL_MOUSEBUTTONDOWN:
            events.push_back(Event {
                .type = EventType::MouseDown,
                .mouse_event = MouseEvent {
                    .x = (float)sdl_event.button.x,
                    .y = (float)sdl_event.button.y,
                    .delta_x = 0,
                    .delta_y = 0,
                    .button = from_sdl_mouse_button(sdl_event.button.button),
                },
            });
            break;
        case SDL_MOUSEBUTTONUP:
            events.push_back(Event {
                .type = EventType::MouseUp,
                .mouse_event = MouseEvent {
                    .x = (float)sdl_event.button.x,
                    .y = (float)sdl_event.button.y,
                    .delta_x = 0,
                    .delta_y = 0,
                    .button = from_sdl_mouse_button(sdl_event.button.button),
                },
            });
            break;
        case SDL_MOUSEMOTION:
            events.push_back(Event {
                .type = EventType::MouseMove,
                .mouse_event = MouseEvent {
                    .x = (float)sdl_event.motion.x,
                    .y = (float)sdl_event.motion.y,
                    .delta_x = (float)sdl_event.motion.xrel,
                    .delta_y = (float)sdl_event.motion.yrel,
                    .button = MouseButton::None,
                },
            });
            break;
        case SDL_MOUSEWHEEL:
            events.push_back(Event {
                .type = EventType::Wheel,
                .wheel_event = WheelEvent {
                    .y = sdl_event.wheel.preciseY,
                },
            });
            break;
        }
    }

    return events;
}

void Window::swap() const
{
    SDL_GL_SwapWindow(window_);
}

} // ui
