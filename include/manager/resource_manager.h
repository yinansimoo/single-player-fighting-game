#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "atlas.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <string>
#include <unordered_map>
#include <stdexcept>

class ResourceManage
{
public:
    static ResourceManage &instance();

    void load(SDL_Renderer *renderer);

    Atlas *find_atlas(const std::string &id) const;
    SDL_Texture *find_texture(const std::string &id) const;
    Mix_Music *find_music(const std::string &id) const;
    Mix_Chunk *find_chunk(const std::string &id) const;
    TTF_Font *get_font() const;
    void clean();
    void load_texture(const std::string &id, const std::string &path);
    void load_atlas(const std::string &id, const char *path_template, int num);
    void load_music(const std::string &id, const std::string &path);
    void load_chunk(const std::string &id, const std::string &path);

private:
    SDL_Renderer *renderer = nullptr;

    std::unordered_map<std::string, Atlas *> atlas_manager;
    std::unordered_map<std::string, SDL_Texture *> texture_manager;
    std::unordered_map<std::string, Mix_Music *> music_manager;
    std::unordered_map<std::string, Mix_Chunk *> chunk_manager;

    TTF_Font *text_font = nullptr;

private:
    ResourceManage() = default;
    ~ResourceManage() = default;
    ResourceManage(const ResourceManage &) = delete;
    ResourceManage &operator=(const ResourceManage &) = delete;

    SDL_Texture *flip_texture(SDL_Texture *src_texture, int num);
    void flip_texture(const std::string &src_id, const std::string &dst_id, int num);
    void flip_atlas(const std::string &src_id, const std::string &dst_id);
};

#endif