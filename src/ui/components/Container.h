#pragma once

#include "../Component.h"

namespace ui {

class Container : public Component {
public:
    explicit Container(std::vector<std::unique_ptr<Component>> children)
        : children_(std::move(children)), focused_(nullptr) {}

    void update(const Timestep& timestep) override;
    Component* select_child(const Event& event) const override;

protected:
    std::vector<std::unique_ptr<Component>> children_;
    Component* focused_;
};

} // ui
