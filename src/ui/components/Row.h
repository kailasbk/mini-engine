#ifndef OPENGL_RENDERER_ROW_H
#define OPENGL_RENDERER_ROW_H

#include "Container.h"

namespace ui {

    class Row : public Container {
    public:
        Row(float height, std::vector<std::unique_ptr<Component>> children)
            : Container(std::move(children)), m_desiredHeight(height) {}

        Size resize(float widthBound, float heightBound) override;

        void reposition(float x, float y) override;

        void draw(RenderList& renderList) const override;

    private:
        float m_desiredHeight;
    };

} // ui

#endif //OPENGL_RENDERER_ROW_H
