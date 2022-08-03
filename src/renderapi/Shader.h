#ifndef OPENGL_RENDERER_SHADER_H
#define OPENGL_RENDERER_SHADER_H


enum class ShaderType {
    Vertex, Fragment, Compute
};

class Shader {
public:
    explicit Shader(ShaderType type) : m_type(type) {}
    virtual ~Shader() = default;

    /**
     * @returns the type of the shader
     */
    ShaderType type() const { return m_type; }
private:
    const ShaderType m_type;
};


#endif //OPENGL_RENDERER_SHADER_H
