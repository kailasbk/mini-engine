#include "Container.h"

namespace ui {

void Container::update(const Timestep& timestep)
{
    // container has no update, only update children
    for (auto& child : children_) {
        child->update(timestep);
    }
}

Component* Container::select_child(const Event& event) const
{
    switch (event.type) {
    case EventType::MouseDown:
    case EventType::MouseUp:
    case EventType::MouseMove:
        for (auto& child : children_) {
            if (child->is_hit(event.mouse_event.x, event.mouse_event.y)) {
                return child.get();
            }
        }
        return nullptr;
    case EventType::KeyDown:
    case EventType::KeyUp:
    case EventType::Wheel:
        return focused_;
    default: // TODO: should not need
        return nullptr;
    }
}

} // ui
