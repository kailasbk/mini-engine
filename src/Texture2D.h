#ifndef OPENGL_RENDERER_TEXTURE2D_H
#define OPENGL_RENDERER_TEXTURE2D_H

class Texture2D {
public:
    enum class Format {
        RGB8, RGBA8,
    };

    /**
     * Constructs a 2D texture of the given dimensions from the given image data and format.
     *
     * @param width the width of the texture, in pixels
     * @param height the height of the texture, in pixels
     * @param format the input format of the image data
     * @param data the pointer to the image data
     */
    Texture2D(int width, int height, Format format, const void* data);

    /**
     * Constructs an empty 2D texture of the given dimensions.
     *
     * @param width the width of the texture, in pixels
     * @param height the height of the texture, in pixels
     */
    Texture2D(int width, int height);

    /**
     * Constructs an empty 2D multisample texture of the given dimensions.
     *
     * @param width the width of the texture, in pixels
     * @param height the height of the texture, in pixels
     * @param numSamples the number of numSamples per pixel
     */
    Texture2D(int width, int height, int numSamples);

    /**
     * Destructs the 2D texture.
     */
    ~Texture2D();

    /**
     * Loads and constructs a texture from the given file.
     *
     * @param filename the file to load the texture from
     * @return a shared pointer to the constructed texture
     */
    static std::shared_ptr<Texture2D> fromFile(const std::string& filename);

    /**
     * Binds the texture to the given texture unit.
     *
     * @param unit the texture unit
     */
    void bind(unsigned int unit) const;

    /**
     * Unbinds the given texture unit.
     *
     * @param unit the texture unit
     */
    static void unbind(unsigned int unit);

    /**
     * @return the id of the texture.
     */
    unsigned int getID() const;

    /**
     * @return the number of samples per pixel, 0 if not a multisample texture.
     */
    int getNumSamples() const;

private:
    unsigned int m_textureID;
    int m_numSamples;
};


#endif //OPENGL_RENDERER_TEXTURE2D_H
