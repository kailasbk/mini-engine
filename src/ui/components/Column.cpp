#include "Column.h"

namespace ui {

    Size Column::resize(float widthBound, float heightBound) {
        m_width = std::min(m_desiredWidth, widthBound);
        m_height = heightBound;

        // resize the children of the column
        for (auto& child: m_children) {
            child->resize(m_width, m_height);
        }

        return Size{m_width, m_height};
    }

    void Column::reposition(float x, float y) {
        // update the position of the column
        m_x = x;
        m_y = y;

        // reposition the children of the column
        float offset = 0;
        for (auto& child: m_children) {
            child->reposition(m_x, m_y + offset);
            offset += 0000; // use size of the component
        }
    }

    void Column::draw(RenderList& renderList) const {
        // render the column's children
        for (auto& child : m_children) {
            child->draw(renderList);
        }

        // render the column as a rect
        renderList.submitRect(RectInfo{
            .position{m_x, m_y, 1.0f},
            .size{m_width, m_height},
            .color{0.0f, 0.0f, 0.0f, 0.0f},
        });
    }

} // ui
