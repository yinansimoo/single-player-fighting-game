#include "atlas.h"

Atlas::~Atlas()
{
    clean();
}

void Atlas::load(SDL_Renderer *renderer, const char *path_template, int num)
{
    clean();

    for (int i = 0; i < num; i++)
    {
        char path_file[256];
        sprintf(path_file, path_template, i + 1);
        SDL_Texture *texture = IMG_LoadTexture(renderer, path_file);
        if (texture != nullptr)
        {
            atlas_list.push_back(texture);
        }
        else
        {
            std::string error = "加载纹理失败: ", path_file;
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "资源错误",
                error.c_str(),
                NULL);
            throw std::runtime_error(error);
        }
    }
}

void Atlas::clean()
{
    for (auto &texture : atlas_list)
    {
        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }
    }
    atlas_list.clear();
}

int Atlas::get_size()
{
    return (int)atlas_list.size();
}

SDL_Texture *Atlas::get_texture(int idx)
{
    if (idx < 0 || idx >= (int)atlas_list.size())
    {
        return nullptr;
    }

    return atlas_list[idx];
}

void Atlas::add_texture(SDL_Texture *texture)
{
    atlas_list.push_back(texture);
}