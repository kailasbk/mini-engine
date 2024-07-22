#include "Pane.h"

namespace ui {

void Pane::update(const Timestep& timestep)
{
    view_->update(timestep);
}

void Pane::handle_event(Event& event)
{
    switch (event.type) {
    case EventType::MouseDown:
        if (title_bar_->is_hit(event.mouse_event.x, event.mouse_event.y)) {
            drag_ = true;
        }
        break;
    case EventType::MouseMove:
        if (drag_) {
            reposition(x_ + event.mouse_event.delta_x, y_ + event.mouse_event.delta_y);
        }
        break;
    case EventType::MouseUp:
        drag_ = false;
        break;
    default:
        break;
    }
}

Component* Pane::select_child(const Event& event) const
{
    switch (event.type) {
    case EventType::MouseMove:
    case EventType::MouseDown:
    case EventType::MouseUp:
        if (view_->is_hit(event.mouse_event.x, event.mouse_event.y)) {
            return view_.get();
        } else {
            return nullptr;
        }
    case EventType::KeyDown:
    case EventType::KeyUp:
    case EventType::Wheel:
        return view_.get();
    default: // TODO: should not need
        return nullptr;
    }
}

Size Pane::resize(float width_bound, float height_bound)
{
    height_bound -= TITLE_HEIGHT;
    return view_->resize(width_bound, height_bound);
}

void Pane::reposition(float x, float y)
{
    view_->reposition(x, y + TITLE_HEIGHT);
}

void Pane::draw(RenderList& render_list) const
{
    // draw the view
    view_->draw(render_list);

    // draw the title bar
    render_list.submit_rect(RectInfo {
        .position = Position {
            .x = x_,
            .y = y_,
            .z = 0.0f,
        },
        .size = Size {
            .width = width_,
            .height = TITLE_HEIGHT,
        },
        .color = Color {
            .r = 0.1f,
            .g = 0.1f,
            .b = 0.1f,
            .a = 1.0f,
        },
    });

    // draw the view background
    render_list.submit_rect(RectInfo {
        .position = Position {
            .x = x_,
            .y = y_ + TITLE_HEIGHT,
            .z = 0.0f,
        },
        .size = Size {
            .width = width_,
            .height = height_ - TITLE_HEIGHT,
        },
        .color = Color {
            .r = 0.2f,
            .g = 0.2f,
            .b = 0.2f,
            .a = 1.0f,
        },
    });
}

} // ui
