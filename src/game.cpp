#include "game.h"
#include "resource_manager.h"
#include "collision_manager.h"
#include "state_machine.h"
#include "character_manager.h"
#include "player_state_node.h"
#include "bullet_time_manager.h"

#include <chrono>
#include <thread>

Game &Game::initstate()
{
    static Game game;
    return game;
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        is_running = false;
    }

    window = SDL_CreateWindow("kd PK hf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not created! SDL_Error: %s\n", SDL_GetError());
        is_running = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not created! SDL_Error: %s\n", SDL_GetError());
        is_running = false;
    }
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

    //  初始化SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG could not initalize! SDL_Error: %s\n", IMG_GetError());
        is_running = false;
    }

    //  初始化SDL_mixer
    if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) != (MIX_INIT_OGG | MIX_INIT_MP3))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix could not initalize! SDL_Error: %s\n", Mix_GetError());
        is_running = false;
    }
    //  打开音频设备
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix could not audio! SDL_Error: %s\n", Mix_GetError());
        is_running = false;
    }
    Mix_AllocateChannels(32);
    Mix_ReserveChannels(2);

    //  设置音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_Volume(-1, MIX_MAX_VOLUME / 2);

    //  初始化字体
    if (TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IIF could not initalize! SDL_Error: %s\n", TTF_GetError());
        is_running = false;
    }

    camera = Camera(renderer);

    try
    {
        ResourceManage::instance().load(renderer);
    }
    catch (const std::exception &error)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "资源错误",
            error.what(),
            NULL);
        is_running = false;
    }

    Mix_PlayMusic(ResourceManage::instance().find_music("bgm"), -1);
}

void Game::run()
{
    using namespace std::chrono;

    init();

    const nanoseconds frame_duration(1000000000 / 144);
    steady_clock::time_point lask_tick = steady_clock::now();

    while (is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> delta = duration<float>(frame_start - lask_tick);

        update(delta.count());

        render();

        lask_tick = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0))
        {
            std::this_thread::sleep_for(sleep_duration);
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);

    draw_background();
    CharacterManager::instance().on_render(camera);
    if (is_visual_box)
    {
        CollisionManager::initstate().on_debug_render(camera);
    }
    draw_hp();

    SDL_RenderPresent(renderer);
}

void Game::draw_background()
{
    SDL_Texture *tex = ResourceManage::instance().find_texture("background");
    if (tex == nullptr)
    {
        return;
    }
    int width, height;
    SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
    SDL_Rect dst = {(windowWidth - width) / 2, (windowHeight - height) / 2, width, height};
    camera.on_render(tex, nullptr, &dst);
}

void Game::draw_text(const char *text, const Vector2 &position)
{
    SDL_Color color = {255, 255, 255, 255};
    TTF_Font *font = ResourceManage::instance().get_font();
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SDL_Rect dst;
    dst.x = position.x;
    dst.y = position.y;
    dst.w = width;
    dst.h = height;
    camera.on_render(texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Game::draw_hp()
{
    SDL_Texture *ui_heart = ResourceManage::instance().find_texture("ui_heart");
    int w, h;
    SDL_QueryTexture(ui_heart, nullptr, nullptr, &w, &h);
    SDL_Rect dst = {0, 10, w, h};
    for (int i = 0; i < CharacterManager::instance().get_player()->get_hp(); i++)
    {
        dst.x = 10 + i * 40;
        camera.on_render(ui_heart, nullptr, &dst);
    }
}

void Game::update(float delta)
{
    //  处理子弹时间
    float scaled_delta = BulletTimeManager::instance().on_update(delta);
    CharacterManager::instance().on_update(scaled_delta);
    CollisionManager::initstate().process_collide();
}

void Game::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_QUIT)
    {
        is_running = false;
    }
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            is_running = false;
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_O)
        {
            is_visual_box = !is_visual_box;
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_F11)
        {
            Uint32 flags = SDL_GetWindowFlags(window);
            if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
            {
                SDL_SetWindowFullscreen(window, 0);
            }
            else
            {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
    }
    CharacterManager::instance().get_player()->on_input(event);
}

void Game::clean()
{
    ResourceManage::instance().clean();

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer *Game::get_renderer() const
{
    return renderer;
}

int Game::get_windowwidth() const
{
    return windowWidth;
}

int Game::get_windowheight() const
{
    return windowHeight;
}
