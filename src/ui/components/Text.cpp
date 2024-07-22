#include "Text.h"

namespace ui {

    Size Text::resize(float width_bound, float height_bound) {
        // compute the size of the text, and clamp to the bounds
        return Size {
            .width = width_,
            .height = height_,
        };
    }

    void Text::reposition(float x, float y) {
        // no children, only reposition text
        x_ = x;
        y_ = y;
    }

    void Text::draw(RenderList& render_list) const {
        // render only the text, there are no children
        render_list.submit_text(TextInfo{
            .position = Position {
                .x = x_,
                .y = y_,
                .z = 1.0f,
            },
            .text = text_,
        });
    }

}
