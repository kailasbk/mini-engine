#include "Component.h"

namespace ui {

    bool Component::isHit(float cursorX, float cursorY) const {
        bool isHitX = m_x <= cursorX && cursorX < m_x + m_width;
        bool isHitY = m_y <= cursorY && cursorY < m_y + m_height;

        return isHitX && isHitY;
    }

    void Component::propagateEvent(Event& event) {
        // starting with empty path with this as root
        std::vector<Component*> path = {};
        Component* component = this;

        // traverse down, generating the path
        while (component != nullptr) {
            path.push_back(component);
            component = component->selectChild(event);
        }

        // bubble back up the path, running handlers
        for (auto iter = path.rbegin(); iter != path.rend(); ++iter) {
            // run the components handler
            component = *iter;
            component->handleEvent(event);

            // end propagation if the handler disables it
            if (!event.propagate) {
                break;
            }
        }
    }

} // ui