#include "Text.h"

namespace ui {

    Size Text::resize(float widthBound, float heightBound) {
        // compute the size of the text, and clamp to the bounds
        return Size{m_width, m_height};
    }

    void Text::reposition(float x, float y) {
        // no children, only reposition text
        m_x = x;
        m_y = y;
    }

    void Text::draw(RenderList& renderList) const {
        // render only the text, there are no children
        renderList.submitText(TextInfo{
            .position{m_x, m_y, 1.0f},
            .text = m_text
        });
    }

}
