#include "Row.h"

namespace ui {

    Size Row::resize(float widthBound, float heightBound) {
        m_width = widthBound;
        m_height = std::min(m_desiredHeight, heightBound);

        // resize the children of the row
        for (auto& child: m_children) {
            child->resize(m_width, m_height);
        }

        return Size{m_width, m_height};
    }

    void Row::reposition(float x, float y) {
        // update the position of the row
        m_x = x;
        m_y = y;

        // reposition the children of the row
        float offset = 0;
        for (auto& child: m_children) {
            child->reposition(m_x + offset, m_y);
            offset += 0000; // use size of the component
        }
    }

    void Row::draw(RenderList& renderList) const {
        // render the row's children
        for (auto& child: m_children) {
            child->draw(renderList);
        }

        // render the row as a rect
        renderList.submitRect(RectInfo{
            .position{m_x, m_y, 1.0f},
            .size{m_width, m_height},
            .color{0.0f, 0.0f, 0.0f, 0.0f},
        });
    }

} // ui
