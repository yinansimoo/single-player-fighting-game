#include "animation.h"

Animation::Animation()
{
    timer.set_on_shot(false);
    timer.set_on_timeout([&]()
                         {
        idx_frame++;
        if(idx_frame >= (int)frame_list.size())
        {
            idx_frame = is_loop?0 : frame_list.size()-1;
            if(!is_loop && on_finished)
            {
                on_finished();
            }
        } });
}

Animation::~Animation()
{
    frame_list.clear();
}

void Animation::reset()
{
    timer.restart();

    idx_frame = 0;
}

void Animation::set_position(const Vector2 &position)
{
    this->position = position;
}

void Animation::set_is_loop(bool flag)
{
    this->is_loop = flag;
}

void Animation::set_interval(float interval)
{
    this->timer.set_wait_time(interval);
}

void Animation::set_on_finished(std::function<void()> on_finished)
{
    this->on_finished = on_finished;
}

void Animation::add_frame(SDL_Texture *texture, int num)
{
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    int len = width / num;
    for (int i = 0; i < num; i++)
    {
        SDL_Rect src;
        src.x = len * i;
        src.y = 0;
        src.w = len;
        src.h = height;
        frame_list.emplace_back(texture, src);
    }
}

void Animation::add_frame(Atlas *atlas)
{
    for (int i = 0; i < atlas->get_size(); i++)
    {
        SDL_Texture *texture = atlas->get_texture(i);
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        SDL_Rect src = {0, 0, width, height};
        frame_list.emplace_back(texture, src);
    }
}

void Animation::on_update(float delta)
{
    timer.update(delta);
}

void Animation::on_render(const Camera &camera) const
{
    const Frame &frame = frame_list[idx_frame];
    SDL_Rect dst;
    dst.x = position.x - frame.src.w / 2;
    dst.y = position.y - frame.src.h / 2;
    dst.w = frame.src.w;
    dst.h = frame.src.h;
    camera.on_render(frame.texture, &frame.src, &dst);
}