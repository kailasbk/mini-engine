#include "Container.h"

namespace ui {

    void Container::update(const Timestep& timestep) {
        // container has no update, only update children
        for (auto& child : m_children) {
            child->update(timestep);
        }
    }

    Component* Container::selectChild(const Event& event) const {
        switch (event.type) {
            case EventType::MouseDown:
            case EventType::MouseUp:
            case EventType::MouseMove:
                for (auto& child : m_children) {
                    if (child->isHit(event.mouseEvent.x, event.mouseEvent.y)) {
                        return child.get();
                    }
                }
                return nullptr;
            case EventType::KeyDown:
            case EventType::KeyUp:
            case EventType::Wheel:
                return m_focused;
            default: // TODO: should not need
                return nullptr;
        }
    }

} // ui
