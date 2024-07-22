#include "Component.h"

namespace ui {

bool Component::is_hit(float cursor_x, float cursor_y) const
{
    bool is_hit_x = x_ <= cursor_x && cursor_y < x_ + width_;
    bool is_hit_y = y_ <= cursor_y && cursor_y < y_ + height_;

    return is_hit_x && is_hit_y;
}

void Component::propagate_event(Event& event)
{
    // starting with empty path with this as root
    std::vector<Component*> path = {};
    Component* component = this;

    // traverse down, generating the path
    while (component != nullptr) {
        path.push_back(component);
        component = component->select_child(event);
    }

    // bubble back up the path, running handlers
    for (auto iter = path.rbegin(); iter != path.rend(); iter++) {
        // run the components handler
        component = *iter;
        component->handle_event(event);

        // end propagation if the handler disables it
        if (!event.propagate) {
            break;
        }
    }
}

} // ui
