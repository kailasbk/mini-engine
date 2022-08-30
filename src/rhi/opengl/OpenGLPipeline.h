#ifndef OPENGL_RENDERER_OPENGLPIPELINE_H
#define OPENGL_RENDERER_OPENGLPIPELINE_H

#include "OpenGLRHI.h"
#include "OpenGLShader.h"

class OpenGLPipeline : public Pipeline, public Resource<GLuint> {
public:
    OpenGLPipeline(GLuint handle, Topology topology, VertexLayout vertexLayout)
            : Pipeline(topology), Resource<GLuint>(handle), m_vertexLayout(std::move(vertexLayout)) {};

    ~OpenGLPipeline() override {
        glDeleteProgram(m_handle);
    }

    const VertexLayout& vertexLayout() {
        return m_vertexLayout;
    }

    constexpr static OpenGLPipeline& from(Pipeline& pipeline) {
        return dynamic_cast<OpenGLPipeline&>(pipeline);
    }

private:
    friend class OpenGLPipelineBuilder;

    VertexLayout m_vertexLayout;
};

class OpenGLPipelineBuilder : public PipelineBuilder {
public:
    OpenGLPipelineBuilder()
            : m_topology(Topology::Triangles), m_vertexShader(nullptr), m_fragmentShader(nullptr),
              m_vertexLayout(nullptr) {};

    PipelineBuilder* setTopology(Topology topology) override {
        m_topology = topology;
        return this;
    }

    PipelineBuilder* setVertexShader(Shader& shader) override {
        m_vertexShader = dynamic_cast<OpenGLShader*>(std::addressof(shader));
        return this;
    }

    PipelineBuilder* setFragmentShader(Shader& shader) override {
        m_fragmentShader = dynamic_cast<OpenGLShader*>(std::addressof(shader));
        return this;
    }

    PipelineBuilder* setVertexLayout(const VertexLayout& layout) override {
        m_vertexLayout = std::addressof(layout);
        return this;
    }

    std::unique_ptr<Pipeline> build() override;

private:
    Topology m_topology;
    OpenGLShader* m_vertexShader;
    OpenGLShader* m_fragmentShader;
    const VertexLayout* m_vertexLayout;
};


#endif //OPENGL_RENDERER_OPENGLPIPELINE_H
