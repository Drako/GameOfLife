#ifndef SDL_HXX
#define SDL_HXX

#include <string>

#include <SDL.h>

class SDL
{
public:
    SDL(Uint32 flags = SDL_INIT_EVERYTHING);
    ~SDL();

    int screen_width() const;
    int screen_height() const;

private:
    int sw_;
    int sh_;
};

class Window
{
public:
    static Uint32 const DEFAULT_WINDOW_FLAGS =
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;

    Window(std::string const & title, int x, int y, int width, int height, Uint32 flags = DEFAULT_WINDOW_FLAGS);
    ~Window();

private:
    SDL_Window * window_;
};

#endif // SDL_HXX
