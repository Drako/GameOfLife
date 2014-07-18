#include "SDL.hxx"
#include "Exception.hxx"

#include <iostream>

#include <cstdlib>

int main() try
{
    SDL sdl;
    Window window("GameOfLife", 0, 0, sdl.screen_width(), sdl.screen_height());
    
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
