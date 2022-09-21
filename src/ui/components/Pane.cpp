#include "Pane.h"

namespace ui {

    void Pane::update(const Timestep& timestep) {
        m_view->update(timestep);
    }

    void Pane::handleEvent(Event& event) {
        switch (event.type) {
            case EventType::MouseDown:
                if (m_titleBar->isHit(event.mouseEvent.x, event.mouseEvent.y)) {
                    m_drag = true;
                }
                break;
            case EventType::MouseMove:
                if (m_drag) {
                    reposition(m_x + event.mouseEvent.deltaX, m_y + event.mouseEvent.deltaY);
                }
                break;
            case EventType::MouseUp:
                m_drag = false;
                break;
            default:
                break;
        }
    }

    Component* Pane::selectChild(const Event& event) const {
        switch (event.type) {
            case EventType::MouseMove:
            case EventType::MouseDown:
            case EventType::MouseUp:
                if (m_view->isHit(event.mouseEvent.x, event.mouseEvent.y)) {
                    return m_view.get();
                } else {
                    return nullptr;
                }
            case EventType::KeyDown:
            case EventType::KeyUp:
            case EventType::Wheel:
                return m_view.get();
            default: // TODO: should not need
                return nullptr;
        }
    }

    Size Pane::resize(float widthBound, float heightBound) {
        heightBound -= titleHeight;
        return m_view->resize(widthBound, heightBound);
    }

    void Pane::reposition(float x, float y) {
        m_view->reposition(x, y + titleHeight);
    }

    void Pane::draw(RenderList& renderList) const {
        // draw the view
        m_view->draw(renderList);

        // draw the title bar
        renderList.submitRect(RectInfo{
            .position{m_x, m_y, 0.0f},
            .size{m_width, titleHeight},
            .color{0.1f, 0.1f, 0.1f, 1.0f},
        });

        // draw the view background
        renderList.submitRect(RectInfo{
            .position{m_x, m_y + titleHeight, 0.0f},
            .size{m_width, m_height - titleHeight},
            .color{0.2f, 0.2f, 0.2f, 1.0f},
        });
    }

} // ui
