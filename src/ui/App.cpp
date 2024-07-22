#include "App.h"

#include "../util/angle.h"
#include "../engine/StaticMeshLoader.h"
#include "../engine/Grid.h"

struct Transform {
    Mat4 transform;
};

struct Motion {
    Vec3 velocity;
};

namespace ui {

    App::App()
        : m_camera(Camera3D::createPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f)), m_renderer(m_camera), m_keys({}) {
        //: m_camera(Camera3D::createOrthographic(16.0f, 9.0f, 0.1f, 100.0f)), m_renderer(m_camera), m_keys({}) {
        RHI& rhi = RHI::current();

        uint32_t internalWidth = 2560; // TODO: sync this with the rest of program / remove magic number
        uint32_t internalHeight = 1440;
        std::unique_ptr<Texture2D> colorAttachment = rhi.createTexture2D(Format::RGBA8, internalWidth, internalHeight);
        std::unique_ptr<Texture2D> depthAttachment = rhi.createTexture2D(Format::D32F, internalWidth, internalHeight);
        m_framebuffer = rhi.createFramebufferBuilder()
            ->setDimensions(internalWidth, internalHeight)
            ->setColorAttachment(std::move(colorAttachment))
            ->setDepthAttachment(std::move(depthAttachment))
            ->build();

        m_middleDown = false;
        m_angle = 45.0f;
        m_vertAngle = 45.0f;
        m_mag = 7.0f;

        updateCamera = [this](){
            float horizMag = m_mag * std::cos(degreesToRadians(m_vertAngle));
            m_camera->moveTo(Vec3(
                horizMag * std::cos(degreesToRadians(m_angle)),
                m_mag * std::sin(degreesToRadians(m_vertAngle)),
                horizMag * std::sin(degreesToRadians(m_angle))
            ));
            m_camera->lookAt({0.0f, 0.0f, 0.0f}, true);
        };
        updateCamera();

        StaticMesh gridMesh = Grid::make(10.0f, 1.0f);

        StaticMeshLoader loader(Material::createDefault());
        StaticMesh monkeyMesh = loader.load("../assets/flat-monkey.obj");

        // create the grid entity
        auto gridEntity = m_scene.createEntity();
        m_scene.addComponent<StaticMesh>(gridEntity);
        m_scene.getComponent<StaticMesh>(gridEntity) = std::move(gridMesh);
        m_scene.addComponent<Transform>(gridEntity);
        m_scene.getComponent<Transform>(gridEntity) = Transform{ .transform = Mat4(1.0f) };

        // create the Suzanne monkey entity
        auto monkeyEntity = m_scene.createEntity();
        m_scene.addComponent<StaticMesh>(monkeyEntity);
        m_scene.getComponent<StaticMesh>(monkeyEntity) = std::move(monkeyMesh);
        m_scene.addComponent<Transform>(monkeyEntity);
        m_scene.getComponent<Transform>(monkeyEntity) = Transform{ .transform = Mat4(1.0f) };
        m_scene.addComponent<Motion>(monkeyEntity);
        m_scene.getComponent<Motion>(monkeyEntity) = Motion{ .velocity = Vec3(0.0f, 0.0f, 0.0f) };

        updateControlSystem = [this](){
            auto meshes = m_scene.view<Motion>();
            meshes.forEach([&](Entity entity, Motion& motion) {
                motion.velocity.x = (m_keys.w) ? .10f : (m_keys.s ? -.10f : 0.0f);
                motion.velocity.y = 0.0f;
                motion.velocity.z = (m_keys.d) ? .10f : (m_keys.a ? -.10f : 0.0f);
            });
        };

        // TODO: these can be made private class methods
        updateMotionSystem = [this](Duration deltaT){
            auto meshes = m_scene.view<StaticMesh, Transform, Motion>();
            meshes.forEach([&](Entity entity, StaticMesh& staticMesh, Transform& transform, Motion& motion){
                Vec3& velocity = motion.velocity;
                transform.transform[3] = transform.transform[3] + Vec4(velocity.x, velocity.y, velocity.z, 0.0f);
            });
        };

        updateRenderSystem = [this](){
            m_renderer.begin(m_framebuffer);

            auto meshes = m_scene.view<StaticMesh, Transform>();
            meshes.forEach([&](Entity entity, StaticMesh& staticMesh, Transform& transform){
                m_renderer.submit(staticMesh, transform.transform);
            });

            m_renderer.end();
        };
    }

    Size App::resize(float maxWidth, float maxHeight) {
        width_ = maxWidth;
        height_ = maxHeight;
        return Size{height_, width_};
    }

    void App::reposition(float x, float y) {
        x_ = x;
        y_ = y;
    }

    void App::handle_event(Event& event) {
        switch (event.type) {
            case EventType::MouseMove:
                if (m_middleDown) {
                    m_angle += event.mouse_event.delta_x / 2;
                    m_vertAngle += event.mouse_event.delta_y / 2;
                    if (m_vertAngle > 89.0f) m_vertAngle = 89.0f;
                    else if (m_vertAngle < -89.0f) m_vertAngle = -89.0f;
                    updateCamera();
                }
                break;
            case EventType::MouseDown:
                if (event.mouse_event.button == MouseButton::Middle) {
                    m_middleDown = true;
                }
                break;
            case EventType::MouseUp:
                if (event.mouse_event.button == MouseButton::Middle) {
                    m_middleDown = false;
                }
                break;
            case EventType::KeyDown:
                switch (event.key_event.key) {
                    case Key::W:
                        m_keys.w = true;
                        break;
                    case Key::A:
                        m_keys.a = true;
                        break;
                    case Key::S:
                        m_keys.s = true;
                        break;
                    case Key::D:
                        m_keys.d = true;
                        break;
                    default:
                        break;
                }
                std::cout << "pressed a key down" << std::endl;
                break;
            case EventType::KeyUp:
                switch (event.key_event.key) {
                    case Key::W:
                        m_keys.w = false;
                        break;
                    case Key::A:
                        m_keys.a = false;
                        break;
                    case Key::S:
                        m_keys.s = false;
                        break;
                    case Key::D:
                        m_keys.d = false;
                        break;
                    default:
                        break;
                }
                std::cout << "unpressed a key" << std::endl;
                break;
            case EventType::Wheel:
                m_mag -= event.wheel_event.y / 2;
                if (m_mag < 0.1f) m_mag = 0.1f;
                updateCamera();
                break;
        }
    }

    void App::update(const Timestep& timestep) {
        updateControlSystem();
        updateMotionSystem(timestep.deltaT);
    }

    void App::draw(RenderList& renderList) const {
        RHI& rhi = RHI::current();

        updateRenderSystem();

        renderList.submit_rect(ui::RectInfo{
            .position{0.0f, 0.0f, +0.1f},
            .size{width_, height_},
            .color{0.1f, 0.1f, 0.1f}
        });

        renderList.submit_image(ui::ImageInfo{
            .position{5.0f, 5.0f, +0.0f},
            .size{1280.0f, 720.0f},
            .texture2d = m_framebuffer->colorAttachment()
        });
    }

} // ui
