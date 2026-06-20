#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "character.h"

class CharacterManager
{
public:
    static CharacterManager &instance();
    Character *get_player();
    Character *get_enemy();

    void on_input(const SDL_Event &event);
    void on_update(float delta);
    void on_render(const Camera &camera);

private:
    CharacterManager();
    ~CharacterManager();
    CharacterManager(const CharacterManager &) = delete;
    CharacterManager &operator=(const CharacterManager &) = delete;

private:
    Character *player = nullptr;
    Character *enemy = nullptr;
};

#endif