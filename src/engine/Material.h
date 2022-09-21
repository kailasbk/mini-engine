#ifndef OPENGL_RENDERER_MATERIAL_H
#define OPENGL_RENDERER_MATERIAL_H

#include "../rhi/RHI.h"

/**
 * A material that references a rendering pipeline, and contains any uniform and texture data
 * to be used by the pipeline.
 */
class Material {
public:
    explicit Material(std::shared_ptr<Pipeline> pipeline)
        : m_pipeline(std::move(pipeline)), m_descriptorSet(RHI::current().createDescriptorSet({})) {
        if (m_pipeline == nullptr) {
            throw std::invalid_argument("Material requires a valid pipeline.");
        }
    }

    /**
     * Sets the model view projection matrix for the material, if one exists. The matrix is
     * generally used to compute vertex positions, but the material is not required to use it.
     *
     * @param modelViewProjection the model view projection matrix
     */
    virtual void setModelViewProjection(const Mat4& modelViewProjection) {
        m_modelViewProjection = modelViewProjection;
    }

    /**
     * Sets the lights for the material. The lights are generally used for computing final
     * fragment color, but the material is not required to use all, or even any, of the lights.
     *
     * @param lightPositions the positions of each light to use
     */
    virtual void setLights(const std::vector<Vec3>& lightPositions) {
        if (!lightPositions.empty()) {
            m_lightPosition = lightPositions[0];
        }
    }

    /**
     * Binds the material to be used for rendering.
     */
    virtual void bind() const {
        RHI& rhi = RHI::current();

        rhi.bindPipeline(*m_pipeline);

        std::vector<Uniform*> uniforms = {
            MatrixUniform<4, 4>::make(m_modelViewProjection),
            VectorUniform<float, 3>::make(m_lightPosition)
        };

        UniformBlock uniformBlock(std::move(uniforms));
        rhi.bindUniforms(uniformBlock);

        rhi.bindDescriptorSet(*m_descriptorSet);
    };

    /**
     * Creates a default material that (describe the rendering)
     *
     * @returns a shared pointer to the material
     */
     static std::shared_ptr<Material> createDefault();

private:
    std::shared_ptr<Pipeline> m_pipeline;
    std::unique_ptr<DescriptorSet> m_descriptorSet;
    Vec3 m_lightPosition;
    Mat4 m_modelViewProjection;
};


#endif //OPENGL_RENDERER_MATERIAL_H
