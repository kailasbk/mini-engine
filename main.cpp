#include "src/ui/Window.h"
#include "src/ui/Renderer.h"
#include "src/ui/App.h"

int main(int argc, char* argv[]) {
    int kWidth = 1290, kHeight = 730;
    ui::Window window(kWidth, kHeight, "OpenGL Test");

    std::unique_ptr<ui::Component> app = std::make_unique<ui::App>();

    ui::Renderer renderer(1290.0f, 730.0f);
    app->reposition(0, 0);
    app->resize((float)window.dimensions().x, (float)window.dimensions().y);

    Timestep timestep = Timestep::start();
    while (!window.shouldClose()) {

        // process pending events
        for (auto event: window.poll()) {
            app->propagateEvent(event);
        }

        // update given time passed
        timestep = Timestep::now(timestep);
        app->update(timestep);

        ui::RenderList list;

        app->draw(list);

        // draw the final ui to default framebuffer
        renderer.render(list);

        window.swap();
    }

    return 0;
}
