#ifndef OPENGL_RENDERER_PROGRAM_H
#define OPENGL_RENDERER_PROGRAM_H

#include <memory>
#include <map>
#include <glm/glm.hpp>
#include "Shader.h"

class Program {
public:
    /**
     * Constructs the program, which starts empty.
     */
    Program();

    /**
     * Destructs the program.
     */
    ~Program();

    /**
     * Adds the given shader to the given stage of the program, replacing any shader in that
     * stage previously. If the shader type does not match the state, nothing happens.
     *
     * @param shader the shader to add to the program
     * @param stage the stage of the program
     */
    void addShader(const std::shared_ptr<Shader>& shader, Shader::Type stage);

    /**
     * Sets a 3-component vector uniform with the given name.
     *
     * @param name the name of the uniform
     * @param matrix the vector
     */
    void setVec3(const std::string& name, const glm::vec3& vector) const;

    /**
     * Sets a 4x4 matrix uniform with the given name.
     *
     * @param name the name of the uniform
     * @param matrix the matrix
     */
    void setMat4(const std::string& name, const glm::mat4& matrix) const;

    /**
     * Links the program, which requires that both vertex and fragment shader have been added.
     */
    void link() const;

    /**
     * Binds the program for use.
     */
    void bind() const;
private:
    unsigned int m_programID;
    std::map<Shader::Type, std::shared_ptr<Shader>> m_shaders;
};


#endif //OPENGL_RENDERER_PROGRAM_H
