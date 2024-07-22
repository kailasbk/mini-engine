#include "Row.h"

namespace ui {

Size Row::resize(float width_bound, float height_bound)
{
    width_ = width_bound;
    height_ = std::min(desired_height_, height_bound);

    // resize the children of the row
    for (auto& child : children_) {
        child->resize(width_, height_);
    }

    return Size {
        .width = width_,
        .height = height_,
    };
}

void Row::reposition(float x, float y)
{
    // update the position of the row
    x_ = x;
    y_ = y;

    // reposition the children of the row
    float offset = 0;
    for (auto& child : children_) {
        child->reposition(x_ + offset, y_);
        offset += 0000; // use size of the component
    }
}

void Row::draw(RenderList& render_list) const
{
    // render the row's children
    for (auto& child : children_) {
        child->draw(render_list);
    }

    // render the row as a rect
    render_list.submit_rect(RectInfo {
        .position = Position {
            .x = x_,
            .y = y_,
            .z = 1.0f,
        },
        .size = Size {
            .width = width_,
            .height = height_,
        },
        .color = Color {
            .r = 0.0f,
            .g = 0.0f,
            .b = 0.0f,
            .a = 0.0f,
        },
    });
}

} // ui
