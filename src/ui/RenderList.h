#pragma once

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
    Texture2D& texture2d;
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
     * @param image_info the information describing the image
     */
    void submit_image(const ImageInfo& image_info) {
        images_.push_back(image_info);
    }

    /**
     * @returns a vector of submitted images to be rendered
     */
    const std::vector<ImageInfo>& images() const {
        return images_;
    }

    /**
     * Submits a string of text to be rendered.
     *
     * @param text_info the information describing the text
     */
    void submit_text(const TextInfo& text_info) {
        texts_.push_back(text_info);
    }

    /**
     * @returns a vector of submitted text segments to be rendered
     */
    const std::vector<TextInfo>& texts() const {
        return texts_;
    }

    /**
     * Submits a colored rectangle to be rendered.
     *
     * @param rect_info the information describing the rectangle
     */
    void submit_rect(const RectInfo& rect_info) {
        rects_.push_back(rect_info);
    }

    /**
     * @returns a vector of submitted rectangles to be rendered
     */
    const std::vector<RectInfo>& rects() const {
        return rects_;
    }

private:
    std::vector<ImageInfo> images_;
    std::vector<TextInfo> texts_;
    std::vector<RectInfo> rects_;
};

} // ui
