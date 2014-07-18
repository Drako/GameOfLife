#include "SDL.hxx"
#include "Exception.hxx"

#include <iostream>

#include <cstdlib>

#include <GL/gl.h>

int main() try
{
    SDL sdl;
    sdl.set_video_mode();
    
    do
    {
        sdl.clear();
        
        glBegin(GL_TRIANGLES);
        
        glColor3f(1.f, .0f, .0f);
        glVertex2f(sdl.width() / 2.f, 10.f);
        
        glColor3f(.0f, 1.f, .0f);
        glVertex2f(10.f, sdl.height() - 10.f);
        
        glColor3f(.0f, .0f, 1.f);
        glVertex2f(sdl.width() - 10.f, sdl.height() - 10.f);
        
        glEnd();
        
        sdl.swap_buffers();
    } while (sdl.handle_events());
    
    return EXIT_SUCCESS;
}
catch (Exception const & ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}
catch (std::exception const & ex)
{
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "An unknown exception occurred!" << std::endl;
    return EXIT_FAILURE;
}
