#ifndef SDL_HXX
#define SDL_HXX

struct SDL_Surface;

class SDL
{
public:
    SDL();
    ~SDL();
    
    void set_video_mode();
    bool handle_events();
    void swap_buffers();
    void clear();
    
    int width() const;
    int height() const;
    
private:
    SDL_Surface * screen_;
};

#endif // SDL_HXX
