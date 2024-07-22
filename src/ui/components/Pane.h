#pragma once

#include "../Component.h"

namespace ui {

class Pane : public Component {
public:
    Pane(std::unique_ptr<Component> titleBar, std::unique_ptr<Component> view)
        : drag_(false), title_bar_(std::move(titleBar)), view_(std::move(titleBar)) {}

    void update(const Timestep& timestep) override;
    void handle_event(Event &event) override;
    Component* select_child(const Event& event) const override;
    Size resize(float widthBound, float heightBound) override;
    void reposition(float x, float y) override;
    void draw(RenderList& renderList) const override;

private:
    static constexpr float TITLE_HEIGHT = 10.0f;

    bool drag_;
    std::unique_ptr<Component> title_bar_;
    std::unique_ptr<Component> view_;
};

} // ui
