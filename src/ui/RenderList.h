#ifndef OPENGL_RENDERER_RENDERLIST_H
#define OPENGL_RENDERER_RENDERLIST_H

#include "../util/Vector.h"
#include "../rhi/Texture2D.h"

/**
 * here is my idea for drawing the 2d elements:
 *
 * break up rendering into processing of several major types
 * each type can be rendered with its own shader, so 1 draw call per type in best case
 * the type would be as follows:
 *  - Image, which would have render an image, or 2d texture, to a rectangular area
 *    - could be used for loaded images, but also for the 3d viewport
 *  - Text, which would render a string of text (start with ascii, maybe utf-8 later) to a rectangular area
 *  - Icon (later), which would render an icon of a given size to a location on the screen
 *  - Rect, which would render a rectangular area of constant color to the screen
 *    - modify later to allow for borders and rounded edges
 * using these types, can basically render anything (given that they are position and shaped properly)
 * every renderable would have to have a screen (x, y) coordinated and z level for layering
 * because rendering is 2d the transforms that need to be used can be relatively trivial
 *
 * each component would write out its renderables of each type with the draw() function.
 * then the renderer can iterate over all renderables of each type, and do significant batch rendering
 *
 * many components won't need to use the gpu directly, the main exception being the 3d viewport for the game
 * which will need to render the contents of its image
 */

namespace ui {

    /**
     * Position in screen coordinates, where (0, 0) is the top-left corner.
     */
    struct Position {
        float x;
        float y;
        float z;
    };

    /**
     * Size in screen coordinates, where width expands to the right and height to the bottom.
     */
    struct Size {
        float width;
        float height;
    };

    /**
     * Color in RGBA format. All component and alpha values are in [0, 1].
     */
    struct Color {
        float r;
        float g;
        float b;
        float a = 1.0f;
    };

    struct ImageInfo {
        Position position;
        Size size;
        Texture2D& texture2D;
    };

    struct TextInfo {
        Position position;
        std::string text;
    };

    struct RectInfo {
        Position position;
        Size size;
        Color color;
    };

    class RenderList {
    public:
        /**
         * Submits an image to be rendered.
         *
         * @param imageInfo the information describing the image
         */
        void submitImage(const ImageInfo& imageInfo) {
            m_images.push_back(imageInfo);
        }

        /**
         * @returns a vector of submitted images to be rendered
         */
        const std::vector<ImageInfo>& images() const {
            return m_images;
        }

        /**
         * Submits a string of text to be rendered.
         *
         * @param textInfo the information describing the text
         */
        void submitText(const TextInfo& textInfo) {
            m_texts.push_back(textInfo);
        }

        /**
         * @returns a vector of submitted text segments to be rendered
         */
        const std::vector<TextInfo>& texts() const {
            return m_texts;
        }

        /**
         * Submits a colored rectangle to be rendered.
         * 
         * @param rectInfo the information describing the rectangle
         */
        void submitRect(const RectInfo& rectInfo) {
            m_rects.push_back(rectInfo);
        }

        /**
         * @returns a vector of submitted rectangles to be rendered
         */
        const std::vector<RectInfo>& rects() const {
            return m_rects;
        }

    private:
        std::vector<ImageInfo> m_images;
        std::vector<TextInfo> m_texts;
        std::vector<RectInfo> m_rects;
    };

} // ui

#endif //OPENGL_RENDERER_RENDERLIST_H
