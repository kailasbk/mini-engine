#ifndef OPENGL_RENDERER_WINDOW_H
#define OPENGL_RENDERER_WINDOW_H


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

    SDL_Window* m_window;
private:
    SDL_GLContext m_context;
    int m_width, m_height;
    static int m_numWindows;
};


#endif //OPENGL_RENDERER_WINDOW_H
