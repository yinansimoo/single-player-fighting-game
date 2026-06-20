#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector2.h"

#include "SDL.h"

class Camera
{
public:
    Camera() = default;
    Camera(SDL_Renderer *renderer);
    ~Camera() = default;

    void reset();
    Vector2 get_position() const;
    Vector2 get_size() const;
    void on_render(SDL_Texture *texture, const SDL_Rect *src, const SDL_Rect *dst) const;
    void on_debug(const SDL_Rect *dst, SDL_Color color) const;
    void post_process(uint8_t alpha) const;

private:
    SDL_Renderer *renderer;
    Vector2 position;
    Vector2 size;
};

#endif