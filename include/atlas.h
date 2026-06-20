#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include <stdexcept>

class Atlas
{
public:
    Atlas() = default;
    ~Atlas();

    void load(SDL_Renderer *renderer, const char *path_template, int num);

    void clean();

    int get_size();

    SDL_Texture *get_texture(int idx);

    void add_texture(SDL_Texture *texture);

private:
    std::vector<SDL_Texture *> atlas_list;
};

#endif