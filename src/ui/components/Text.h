#ifndef OPENGL_RENDERER_TEXT_H
#define OPENGL_RENDERER_TEXT_H

#include "../Component.h"

namespace ui {

    class Text : public Component {
    public:
        explicit Text(std::string text) : m_text(std::move(text)) {}

        void draw(RenderList& renderList) const override;

        Size resize(float widthBound, float heightBound) override;

        void reposition(float x, float y) override;

    private:
        std::string m_text;
    };

}


#endif //OPENGL_RENDERER_TEXT_H
