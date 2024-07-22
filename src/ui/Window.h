#pragma once

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
        return {(uint32_t)width_, (uint32_t)height_};
    };

    /**
     * Polls for the window's events and returns all that are pending.
     *
     * @returns a vector of unhandled events
     */
    std::vector<Event> poll();

    /**
     * @returns whether this window should close (e.g. the user pressed the close button)
     */
    bool should_close() const {
        return should_close_;
    };

    /**
     * Swaps the window images, and presents the next image in the swapchain.
     */
    void swap() const;

private:
    static int num_windows_;

    SDL_Window* window_;
    SDL_GLContext context_;
    bool should_close_;
    int width_, height_;
};

} // ui
