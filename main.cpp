#include "src/ui/Window.h"
#include "src/ui/Renderer.h"
#include "src/ui/App.h"

int main(int argc, char* argv[])
{
    int width = 1290, height = 730;
    ui::Window window(width, height, "OpenGL Test");

    std::unique_ptr<ui::Component> app = std::make_unique<ui::App>();

    ui::Renderer renderer((float)width, (float)height);
    app->reposition(0, 0);
    app->resize((float)window.dimensions().x, (float)window.dimensions().y);

    Timestep timestep = Timestep::start();
    while (!window.should_close()) {

        // process pending events
        for (auto event: window.poll()) {
            app->propagate_event(event);
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
