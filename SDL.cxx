#include "SDL.hxx"
#include "Exception.hxx"

#include <SDL.h>
#include <SDL_opengl.h>

SDL::SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw Exception() << "SDL_Init: " << SDL_GetError();
}

SDL::~SDL()
{
    SDL_Quit();
}

void SDL::set_video_mode()
{
    auto info = SDL_GetVideoInfo();
    int width = info->current_w, height = info->current_h;
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    screen_ = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN | SDL_OPENGL);
    if (!screen_)
        throw Exception() << "SDL_SetVideoMode: " << SDL_GetError();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(.0, width, height, .0);
}

bool SDL::handle_events()
{
    SDL_Event evt;
    
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            {
                switch (evt.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return false;
                    break;
                }
            } break;
        }
    }
    
    return true;
}

void SDL::swap_buffers()
{
    SDL_GL_SwapBuffers();
}

void SDL::clear()
{
    glClearColor(.0f, .0f, .0f, .0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int SDL::width() const
{
    return screen_->w;
}

int SDL::height() const
{
    return screen_->h;
}
