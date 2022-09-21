#ifndef OPENGL_RENDERER_APP_H
#define OPENGL_RENDERER_APP_H

#include "Component.h"
#include "../engine/Camera3D.h"
#include "../rhi/RHI.h"
#include "../engine/ecs/Scene.h"
#include "../engine/Renderer3D.h"

namespace ui {

    class App : public Component {
    public:
        App();
        ~App() override = default;

        void update(const Timestep& timestep) override;
        void handleEvent(Event& event) override;
        Size resize(float maxWidth, float maxHeight) override;
        void reposition(float x, float y) override;
        void draw(RenderList& renderList) const override;

    private:
        bool m_middleDown;
        float m_angle;
        float m_vertAngle;
        float m_mag;
        std::shared_ptr<Camera3D> m_camera;
        std::function<void()> updateCamera;

        Renderer3D m_renderer;
        Scene m_scene;
        std::function<void()> updateRenderSystem;
        std::function<void(Duration)> updateMotionSystem;
        std::function<void()> updateControlSystem;

        struct {
            bool w;
            bool a;
            bool s;
            bool d;
        } m_keys;

        std::shared_ptr<Framebuffer> m_framebuffer;
    };

} // ui

#endif //OPENGL_RENDERER_APP_H
