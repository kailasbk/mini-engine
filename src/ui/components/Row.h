#pragma once

#include "Container.h"

namespace ui {

    class Row : public Container {
    public:
        Row(float height, std::vector<std::unique_ptr<Component>> children)
            : Container(std::move(children)), desired_height_(height) {}

        Size resize(float width_bound, float height_bound) override;
        void reposition(float x, float y) override;
        void draw(RenderList& render_list) const override;

    private:
        float desired_height_;
    };

} // ui
