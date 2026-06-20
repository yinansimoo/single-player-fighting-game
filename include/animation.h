#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "atlas.h"
#include "Vector2.h"
#include "timer.h"
#include "camera.h"

class Animation
{
public:
    Animation();
    ~Animation();

    void reset();

    void set_position(const Vector2 &position);
    void set_is_loop(bool flag);
    void set_interval(float interval);
    void set_on_finished(std::function<void()> on_finished);

    void add_frame(SDL_Texture *texture, int num);
    void add_frame(Atlas *atlas);
    void on_update(float delta);
    void on_render(const Camera &camera) const;

private:
    struct Frame
    {
        SDL_Texture *texture = nullptr;
        SDL_Rect src;

        Frame() = default;
        Frame(SDL_Texture *texture, const SDL_Rect &src) : texture(texture), src(src)
        {
        }
        ~Frame() = default;
    };

private:
    Vector2 position;
    Timer timer;
    bool is_loop = false;
    int idx_frame = 0;
    std::vector<Frame> frame_list;
    std::function<void()> on_finished;
};

#endif