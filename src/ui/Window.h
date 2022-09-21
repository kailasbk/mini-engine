#ifndef OPENGL_RENDERER_WINDOW_H
#define OPENGL_RENDERER_WINDOW_H

#include "../util/Vector.h"
#include "Event.h"

namespace ui {

    class Window {
    public:
        /**
         * Constructs an window with the given width, height, and title.
         *
         * @param width the width of the window, in pixels
         * @param height the height of the window, in pixels
         * @param title the title of the window
         */
        Window(int width, int height, const std::string& title);

        /**
         * Destructs the window.
         */
        ~Window();

        /**
         * @returns the 2d dimensions of the window, in pixels
         */
        Vector<uint32_t, 2> dimensions() const {
            return {(uint32_t)m_width, (uint32_t)m_height};
        };

        /**
         * Polls for the window's events and returns all that are pending.
         *
         * @returns a vector of unhandled events
         */
        std::vector<ui::Event> poll();

        /**
         * @returns whether this window should close (e.g. the user pressed the close button)
         */
        bool shouldClose() const {
            return m_shouldClose;
        };

        /**
         * Swaps the window images, and presents the next image in the swapchain.
         */
        void swap() const;

    private:
        SDL_Window* m_window;
        SDL_GLContext m_context;
        bool m_shouldClose;
        int m_width, m_height;
        static int m_numWindows;
    };

} // ui

#endif //OPENGL_RENDERER_WINDOW_H
