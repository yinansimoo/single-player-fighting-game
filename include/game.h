#ifndef _GAME_H_
#define _GAME_H_

#include "camera.h"
#include "character.h"

class Game
{
public:
    static Game &initstate();

    void init();
    void run();
    void clean();

    void render();
    void update(float delta);
    void handleEvent(const SDL_Event &event);

    void draw_background();
    void draw_text(const char *text, const Vector2 &position);
    void draw_hp();

    SDL_Renderer *get_renderer() const;
    int get_windowwidth() const;
    int get_windowheight() const;

private:
    Game() = default;
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    SDL_Window *window;
    SDL_Renderer *renderer;
    int windowWidth = 1280;
    int windowHeight = 720;

    Camera camera;
    bool is_running = true;
    bool is_visual_box = false;
};

#endif