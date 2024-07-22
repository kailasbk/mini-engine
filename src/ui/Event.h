#pragma once

namespace ui {

enum class MouseButton {
    None, Left, Right, Middle
};

struct MouseEvent {
    const float x, y;             // absolute in the window, in pixels
    const float delta_x, delta_y; // since last mouse move, in pixels
    const MouseButton button;     // button pressed/released for event
    // TODO: add information about specific buttons being pressed
};

enum class Key {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Space, Tab, Backspace, Return, Escape, Minus, Equals, CapsLock,
    LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Grave, Comma, Period, Slash,
    Left, Right, Up, Down,
    Unsupported,
};

struct KeyEvent {
    const Key key;
    // TODO: add information about mods
};

struct WheelEvent {
    const float y;
};

// not used in the event union, to be added later
struct TextEvent {
    const std::string text;
};

enum class EventType {
    MouseMove, MouseDown, MouseUp, KeyDown, KeyUp, Wheel
};

struct Event {
    const EventType type;
    const union {
        MouseEvent mouse_event;
        KeyEvent key_event;
        WheelEvent wheel_event;
    };
    bool propagate = true;
};

} // ui
