#include "camera.h"

Camera::Camera(SDL_Renderer *renderer) : renderer(renderer)
{

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    size = Vector2((float)width, (float)height);

    position = Vector2(0, 0);
}

void Camera::reset()
{
    position = Vector2(0, 0);
}

Vector2 Camera::get_position() const
{
    return position;
}

Vector2 Camera::get_size() const
{
    return size;
}

void Camera::on_render(SDL_Texture *texture, const SDL_Rect *src, const SDL_Rect *dst) const
{
    if (texture == nullptr || dst == nullptr)
    {
        return;
    }
    SDL_Rect win_dst = *dst;
    win_dst.x -= position.x;
    win_dst.y -= position.y;
    SDL_RenderCopy(renderer, texture, src, &win_dst);
}

void Camera::on_debug(const SDL_Rect *dst, SDL_Color color) const
{
    SDL_Rect box_rect = *dst;
    box_rect.x -= position.x;
    box_rect.y -= position.y;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &box_rect);
}

void Camera::post_process(uint8_t alpha) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);

    SDL_RenderFillRect(renderer, NULL);
}