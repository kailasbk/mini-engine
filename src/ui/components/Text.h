#pragma once

#include "../Component.h"

namespace ui {

    class Text : public Component {
    public:
        explicit Text(std::string text) : text_(std::move(text)) {}

        void draw(RenderList& render_list) const override;
        Size resize(float width_bound, float height_bound) override;
        void reposition(float x, float y) override;

    private:
        std::string text_;
    };

}
