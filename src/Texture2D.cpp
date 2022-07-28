#include "Texture2D.h"

Texture2D::Texture2D(int width, int height, Format format, const void* data)
        : m_textureID(0), m_numSamples(0) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);

    // set texture parameters
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    GLenum glFormat;
    switch (format) {
        case Format::RGB8:
            glFormat = GL_RGB;
            break;
        case Format::RGBA8:
            glFormat = GL_RGBA;
            break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(int width, int height)
        : m_textureID(0), m_numSamples(0) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);

    // set texture parameters
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTextureStorage2D(m_textureID, 1, GL_RGBA8, width, height);
}

Texture2D::Texture2D(int width, int height, int numSamples)
        : m_textureID(0), m_numSamples(numSamples) {
    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_textureID);

    // set texture parameters
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTextureStorage2DMultisample(m_textureID, m_numSamples, GL_RGBA8, width, height, GL_TRUE);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_textureID);
}

std::shared_ptr<Texture2D> Texture2D::fromFile(const std::string& filename) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    void* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    Format format;
    if (channels == 3) {
        format = Format::RGB8;
    } else if (channels == 4) {
        format = Format::RGBA8;
    }

    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(width, height, format, data);
    stbi_image_free(data);
    return texture;
}

void Texture2D::bind(unsigned int unit) const {
    glBindTextureUnit(unit, m_textureID);
}

void Texture2D::unbind(unsigned int unit) {
    glBindTextureUnit(unit, 0);
}

unsigned int Texture2D::getID() const {
    return m_textureID;
}

int Texture2D::getNumSamples() const {
    return m_numSamples;
}
