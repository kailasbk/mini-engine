#ifndef OPENGL_RENDERER_CONTAINER_H
#define OPENGL_RENDERER_CONTAINER_H

#include "../Component.h"

namespace ui {

    class Container : public Component {
    public:
        explicit Container(std::vector<std::unique_ptr<Component>> children)
            : m_children(std::move(children)), m_focused(nullptr) {}

        void update(const Timestep& timestep) override;

        Component* selectChild(const Event& event) const override;

    protected:
        std::vector<std::unique_ptr<Component>> m_children;
        Component* m_focused;
    };

} // ui

#endif //OPENGL_RENDERER_CONTAINER_H
