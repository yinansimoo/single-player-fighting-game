#include "resource_manager.h"

void ResourceManage::clean()
{
    for (auto &it : texture_manager)
    {
        if (it.second != nullptr)
        {
            SDL_DestroyTexture(it.second);
        }
    }
    texture_manager.clear();

    for (auto &it : atlas_manager)
    {
        delete it.second;
    }
    atlas_manager.clear();

    for (auto &it : music_manager)
    {
        if (it.second != nullptr)
        {
            Mix_FreeMusic(it.second);
        }
    }
    music_manager.clear();

    for (auto &it : chunk_manager)
    {
        if (it.second != nullptr)
        {
            Mix_FreeChunk(it.second);
        }
    }
    chunk_manager.clear();

    if (text_font != nullptr)
    {
        TTF_CloseFont(text_font);
    }
}

ResourceManage &ResourceManage::instance()
{
    static ResourceManage manager;
    return manager;
}

void ResourceManage::load_texture(const std::string &id, const std::string &path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr)
    {
        std::string error = "加载失败：" + path;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    texture_manager.emplace(id, texture);
}

void ResourceManage::load_atlas(const std::string &id, const char *path_template, int num)
{
    Atlas *atlas = new Atlas();
    atlas->load(renderer, path_template, num);
    atlas_manager.emplace(id, atlas);
}

void ResourceManage::load_music(const std::string &id, const std::string &path)
{
    Mix_Music *music = Mix_LoadMUS(path.c_str());
    if (music == nullptr)
    {
        std::string error = "加载失败：" + path;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    music_manager.emplace(id, music);
}

void ResourceManage::load_chunk(const std::string &id, const std::string &path)
{
    Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
    if (chunk == nullptr)
    {
        std::string error = "加载失败：" + path;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    chunk_manager.emplace(id, chunk);
}

void ResourceManage::load(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    //  加载图片资源
    {
        //  背景
        load_texture("background", "resources/background.png");
        //  ui
        load_texture("ui_heart", "resources/ui_heart.png");

        //  玩家动作（右
        load_texture("player_attack_right", "resources/player/attack.png");
        load_texture("player_dead_right", "resources/player/dead.png");
        load_texture("player_fall_right", "resources/player/fall.png");
        load_texture("player_idle_right", "resources/player/idle.png");
        load_texture("player_jump_right", "resources/player/jump.png");
        load_texture("player_roll_right", "resources/player/roll.png");
        load_texture("player_run_right", "resources/player/run.png");

        //  玩家动作（左
        flip_texture("player_attack_right", "player_attack_left", 5);
        flip_texture("player_dead_right", "player_dead_left", 6);
        flip_texture("player_fall_right", "player_fall_left", 5);
        flip_texture("player_idle_right", "player_idle_left", 5);
        flip_texture("player_jump_right", "player_jump_left", 5);
        flip_texture("player_roll_right", "player_roll_left", 7);
        flip_texture("player_run_right", "player_run_left", 10);

        //  特效
        load_texture("player_vfx_attack_right", "resources/player/vfx_attack_right.png");
        load_texture("player_vfx_attack_left", "resources/player/vfx_attack_left.png");
        load_texture("player_vfx_attack_up", "resources/player/vfx_attack_up.png");
        load_texture("player_vfx_attack_down", "resources/player/vfx_attack_down.png");
        load_texture("player_vfx_jump", "resources/player/vfx_jump.png");
        load_texture("player_vfx_land", "resources/player/vfx_land.png");
    }

    //  加载图集
    {
        //  陷阱
        load_atlas("barb_break", "resources/enemy/barb_break/%d.png", 3);
        load_atlas("barb_loose", "resources/enemy/barb_loose/%d.png", 5);
        load_atlas("silk", "resources/enemy/silk/%d.png", 9);
        load_atlas("sword_left", "resources/enemy/sword/%d.png", 3);
        flip_atlas("sword_left", "sword_right");

        //  敌人动作（左
        load_atlas("enemy_aim_left", "resources/enemy/aim/%d.png", 9);
        load_atlas("enemy_dash_in_air_left", "resources/enemy/dash_in_air/%d.png", 2);
        load_atlas("enemy_dash_on_floor_left", "resources/enemy/dash_on_floor/%d.png", 2);
        load_atlas("enemy_fall_left", "resources/enemy/fall/%d.png", 4);
        load_atlas("enemy_idle_left", "resources/enemy/idle/%d.png", 6);
        load_atlas("enemy_jump_left", "resources/enemy/jump/%d.png", 8);
        load_atlas("enemy_run_left", "resources/enemy/run/%d.png", 8);
        load_atlas("enemy_squat_left", "resources/enemy/squat/%d.png", 10);
        load_atlas("enemy_throw_barb_left", "resources/enemy/throw_barb/%d.png", 8);
        load_atlas("enemy_throw_silk_left", "resources/enemy/throw_silk/%d.png", 17);
        load_atlas("enemy_throw_sword_left", "resources/enemy/throw_sword/%d.png", 16);

        //  敌人动作（右
        flip_atlas("enemy_aim_left", "enemy_aim_right");
        flip_atlas("enemy_dash_in_air_left", "enemy_dash_in_air_right");
        flip_atlas("enemy_dash_on_floor_left", "enemy_dash_on_floor_right");
        flip_atlas("enemy_fall_left", "enemy_fall_right");
        flip_atlas("enemy_idle_left", "enemy_idle_right");
        flip_atlas("enemy_jump_left", "enemy_jump_right");
        flip_atlas("enemy_run_left", "enemy_run_right");
        flip_atlas("enemy_squat_left", "enemy_squat_right");
        flip_atlas("enemy_throw_barb_left", "enemy_throw_barb_right");
        flip_atlas("enemy_throw_silk_left", "enemy_throw_silk_right");
        flip_atlas("enemy_throw_sword_left", "enemy_throw_sword_right");

        //  特效(左
        load_atlas("enemy_vfx_dash_in_air_left", "resources/enemy/vfx_dash_in_air/%d.png", 5);
        load_atlas("enemy_vfx_dash_on_floor_left", "resources/enemy/vfx_dash_on_floor/%d.png", 6);

        //  特效(右
        flip_atlas("enemy_vfx_dash_in_air_left", "enemy_vfx_dash_in_air_right");
        flip_atlas("enemy_vfx_dash_on_floor_left", "enemy_vfx_dash_on_floor_right");
    }

    //  音乐
    {
        load_music("bgm", "resources/audio/bgm.mp3");
    }

    //  音效
    {
        load_chunk("barb_break", "resources/audio/barb_break-soConvert.wav");
        load_chunk("bullet_time", "resources/audio/bullet_time-soConvert.wav");
        load_chunk("enemy_dash", "resources/audio/enemy_dash-soConvert.wav");
        load_chunk("enemy_hurt_1", "resources/audio/enemy_hurt_1-soConvert.wav");
        load_chunk("enemy_hurt_2", "resources/audio/enemy_hurt_2-soConvert.wav");
        load_chunk("enemy_hurt_3", "resources/audio/enemy_hurt_3-soConvert.wav");
        load_chunk("enemy_run", "resources/audio/enemy_run-soConvert.wav");
        load_chunk("enemy_throw_barbs", "resources/audio/enemy_throw_barbs-soConvert.wav");
        load_chunk("enemy_throw_silk", "resources/audio/enemy_throw_silk-soConvert.wav");
        load_chunk("enemy_throw_sword", "resources/audio/enemy_throw_sword-soConvert.wav");

        load_chunk("player_attack_1", "resources/audio/player_attack_1-soConvert.wav");
        load_chunk("player_attack_2", "resources/audio/player_attack_2-soConvert.wav");
        load_chunk("player_attack_3", "resources/audio/player_attack_3-soConvert.wav");
        load_chunk("player_dead", "resources/audio/player_dead-soConvert.wav");
        load_chunk("player_hurt", "resources/audio/player_hurt-soConvert.wav");
        load_chunk("player_jump", "resources/audio/player_jump-soConvert.wav");
        load_chunk("player_land", "resources/audio/player_land-soConvert.wav");
        load_chunk("player_roll", "resources/audio/player_roll-soConvert.wav");
        load_chunk("player_run", "resources/audio/player_run-soConvert.wav");
    }

    //  字体
    {
        text_font = TTF_OpenFont("resources/IPix.ttf", 24);
    }
}

Atlas *ResourceManage::find_atlas(const std::string &id) const
{
    auto it = atlas_manager.find(id);

    if (it == atlas_manager.end())
    {
        return nullptr;
    }

    return it->second;
}

SDL_Texture *ResourceManage::find_texture(const std::string &id) const
{
    auto it = texture_manager.find(id);
    if (it == texture_manager.end())
    {
        return nullptr;
    }

    return it->second;
}

Mix_Music *ResourceManage::find_music(const std::string &id) const
{
    auto it = music_manager.find(id);
    if (it == music_manager.end())
    {
        return nullptr;
    }

    return it->second;
}

Mix_Chunk *ResourceManage::find_chunk(const std::string &id) const
{
    auto it = chunk_manager.find(id);
    if (it == chunk_manager.end())
    {
        return nullptr;
    }

    return it->second;
}

TTF_Font *ResourceManage::get_font() const
{
    return text_font;
}

SDL_Texture *ResourceManage::flip_texture(SDL_Texture *src_text, int num)
{
    if (!src_text || num <= 0)
    {
        return nullptr;
    }

    int sheet_w, sheet_h;
    Uint32 format;
    if (SDL_QueryTexture(src_text, &format, nullptr, &sheet_w, &sheet_h) != 0)
    {
        return nullptr;
    }
    int frame_w = sheet_w / num;

    SDL_Texture *target = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, sheet_w, sheet_h);
    if (!target)
    {
        return nullptr;
    }

    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);

    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, target);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < num; ++i)
    {
        SDL_Rect src = {i * frame_w, 0, frame_w, sheet_h};
        SDL_Rect dst = {i * frame_w, 0, frame_w, sheet_h};
        SDL_RenderCopyEx(renderer, src_text, &src, &dst, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
    }

    SDL_Surface *temp_surf = SDL_CreateRGBSurfaceWithFormat(0, sheet_w, sheet_h, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, temp_surf->pixels, temp_surf->pitch);

    SDL_Texture *final_texture = SDL_CreateTextureFromSurface(renderer, temp_surf);

    SDL_SetRenderTarget(renderer, old_target);
    SDL_FreeSurface(temp_surf);
    SDL_DestroyTexture(target);

    return final_texture;
}

void ResourceManage::flip_texture(const std::string &src_id, const std::string &dst_id, int num)
{
    auto it = texture_manager.find(src_id);
    if (it == texture_manager.end())
    {
        std::string error = "加载失败：" + src_id + "not in atlas_manager";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    SDL_Texture *flipped = flip_texture(it->second, num);
    if (flipped == nullptr)
    {
        std::string error = "加载失败：" + dst_id + "is nullptr";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }
    texture_manager.emplace(dst_id, flipped);
}

void ResourceManage::flip_atlas(const std::string &src_id, const std::string &dst_id)
{
    auto it = atlas_manager.find(src_id);
    if (it == atlas_manager.end())
    {
        std::string error = "加载失败：" + src_id + "not in atlas_manager";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    Atlas *src_atlas = it->second;
    Atlas *dst_atlas = new Atlas();
    for (int i = 0; i < src_atlas->get_size(); i++)
    {
        SDL_Texture *flipped = flip_texture(src_atlas->get_texture(i), 1);
        if (flipped == nullptr)
        {
            std::string error = "加载失败：" + dst_id + "is nullptr";

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "资源错误",
                error.c_str(),
                NULL);

            throw std::runtime_error(error);
        }
        dst_atlas->add_texture(flipped);
    }
    atlas_manager.emplace(dst_id, dst_atlas);
}
