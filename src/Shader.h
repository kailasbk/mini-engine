#ifndef OPENGL_RENDERER_SHADER_H
#define OPENGL_RENDERER_SHADER_H

#include <string>
#include <memory>

class Shader {
public:
    enum class Type {
        Vertex, Fragment, Undefined
    };

    /**
     * Constructs the shader of the given type from the given source.
     *
     * @param source the shader source
     * @param type the shader type
     */
    Shader(const char* source, Type type);

    /**
     * Destructs the shader.
     */
    ~Shader();

    /**
     * Loads and constructs a shader from the given file. The shader's type is inferred
     * from extension of the file (.frag or .vert).
     *
     * @param filename the file to load the shader source from
     * @return the contructed shader
     */
    static std::shared_ptr<Shader> fromFile(const std::string& filename);

    /**
     * @return the id of the shader
     */
    unsigned int getID() const;

    /**
     * @return the type of the shader
     */
    Type getType() const;
private:
    unsigned int m_shaderID;
    const Type m_type;
};


#endif //OPENGL_RENDERER_SHADER_H
