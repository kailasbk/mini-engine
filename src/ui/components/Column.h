#pragma once

#include "Container.h"

namespace ui {

class Column : public Container {
public:
    Column(float width, std::vector<std::unique_ptr<Component>> children);

    Size resize(float width_bound, float height_bound) override;
    void reposition(float x, float y) override;
    void draw(RenderList& render_list) const override;

private:
    float desired_width_;
};

} // ui
