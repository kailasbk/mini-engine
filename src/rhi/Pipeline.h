#ifndef OPENGL_RENDERER_PIPELINE_H
#define OPENGL_RENDERER_PIPELINE_H

#include "Shader.h"
#include "VertexLayout.h"

enum class Topology {
    Points, Lines, Triangles
};

class Pipeline {
public:
    explicit Pipeline(Topology topology) : m_topology(topology) {};

    virtual ~Pipeline() = default;

    /**
     * @returns the topology used by the pipeline for input assembly
     */
    Topology topology() const {
        return m_topology;
    }
private:
    Topology m_topology;
};

class PipelineBuilder {
public:
    virtual ~PipelineBuilder() = default;

    virtual PipelineBuilder* setTopology(Topology topology) = 0;

    virtual PipelineBuilder* setVertexShader(Shader& shader) = 0;

    virtual PipelineBuilder* setFragmentShader(Shader& shader) = 0;

    virtual PipelineBuilder* setVertexLayout(const VertexLayout& layout) = 0;

    virtual std::unique_ptr<Pipeline> build() = 0;
};


#endif //OPENGL_RENDERER_PIPELINE_H
