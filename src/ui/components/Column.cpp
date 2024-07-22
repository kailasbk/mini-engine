#include "Column.h"

namespace ui {

Column::Column(float width, std::vector<std::unique_ptr<Component>> children)
    : Container(std::move(children)), desired_width_(width)
{
}

Size Column::resize(float width_bound, float height_bound)
{
    width_ = std::min(desired_width_, width_bound);
    height_ = height_bound;

    // resize the children of the column
    for (auto& child : children_) {
        child->resize(width_, height_);
    }

    return Size {
        .width = width_,
        .height = height_,
    };
}

void Column::reposition(float x, float y)
{
    // update the position of the column
    x = x_;
    y = y_;

    // reposition the children of the column
    float offset = 0;
    for (auto& child : children_) {
        child->reposition(x_, y_ + offset);
        offset += 0000; // use size of the component
    }
}

void Column::draw(RenderList& render_list) const
{
    // render the column's children
    for (auto& child : children_) {
        child->draw(render_list);
    }

    // render the column as a rect
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
