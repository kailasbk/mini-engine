#ifndef OPENGL_RENDERER_PANE_H
#define OPENGL_RENDERER_PANE_H

#include "../Component.h"

namespace ui {

    class Pane : public Component {
    public:
        Pane(std::unique_ptr<Component> titleBar, std::unique_ptr<Component> view)
            : m_drag(false), m_titleBar(std::move(titleBar)), m_view(std::move(titleBar)) {}

        void update(const Timestep& timestep) override;

        void handleEvent(Event &event) override;

        Component* selectChild(const Event& event) const override;

        Size resize(float widthBound, float heightBound) override;

        void reposition(float x, float y) override;

        void draw(RenderList& renderList) const override;

    private:
        constexpr static float titleHeight = 10.0f;
        bool m_drag;
        std::unique_ptr<Component> m_titleBar;
        std::unique_ptr<Component> m_view;
    };

} // ui


#endif //OPENGL_RENDERER_PANE_H
