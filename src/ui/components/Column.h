#ifndef OPENGL_RENDERER_COLUMN_H
#define OPENGL_RENDERER_COLUMN_H

#include "Container.h"

namespace ui {

    class Column : public Container {
    public:
        Column(float width, std::vector<std::unique_ptr<Component>> children)
            : Container(std::move(children)), m_desiredWidth(width) {}

        Size resize(float widthBound, float heightBound) override;

        void reposition(float x, float y) override;

        void draw(RenderList& renderList) const override;

    private:
        float m_desiredWidth;
    };

} // ui

#endif //OPENGL_RENDERER_COLUMN_H
