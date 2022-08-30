#ifndef OPENGL_RENDERER_TEXTURE2D_H
#define OPENGL_RENDERER_TEXTURE2D_H

#include "Format.h"
#include "../util/Vector.h"

// TODO: add enum for sampling types

class Texture2D {
public:
    Texture2D(Format format, uint32_t width, uint32_t height) : m_format(format), m_width(width), m_height(height) {}
    virtual ~Texture2D() = default;

    /**
     * @returns the 3d dimensions of the texture
     */
    Vector<uint32_t, 3> dimensions() const {
        return {m_width, m_height, 1};
    };

    /**
     * @returns the format of the pixels in the texture
     */
    Format format() const {
        return m_format;
    }

    /**
     * @returns the number of samples per pixel in the texture
     */
    uint32_t numSamples() const {
        return 1;
    };
private:
    const Format m_format;
    const uint32_t m_width;
    const uint32_t m_height;
};


#endif //OPENGL_RENDERER_TEXTURE2D_H
