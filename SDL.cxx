#include "SDL.hxx"
#include "Exception.hxx"

#include <iostream>

SDL::SDL(Uint32 flags /* = SDL_INIT_EVERYTHING */)
{
    if (SDL_Init(flags) != 0)
        throw Exception() << "SDL_Init: " << SDL_GetError();


}

SDL::~SDL()
{
    SDL_Quit();
}

Window::Window(std::string const & title, int x, int y, int width, int height, Uint32 flags /* = DEFAULT_WINDOW_FLAGS */)
    : window_(SDL_CreateWindow(title.c_str(), x, y, width, height, flags))
{
    if (!window_)
        throw Exception() << "SDL_CreateWindow: " << SDL_GetError();
}

Window::~Window()
{
    SDL_DestroyWindow(window_);
}
