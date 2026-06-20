#include "player.h"
#include "enemy.h"
#include "character_manager.h"
#include "bullet_time_manager.h"

CharacterManager &CharacterManager::instance()
{
    static CharacterManager manager;
    return manager;
}

CharacterManager::CharacterManager()
{
    player = new Player();
    enemy = new Enemy();
}

CharacterManager::~CharacterManager()
{
    delete player;
    delete enemy;
}

Character *CharacterManager::get_player()
{
    return player;
}

Character *CharacterManager::get_enemy()
{
    return enemy;
}

void CharacterManager::on_input(const SDL_Event &event)
{
    player->on_input(event);
}

void CharacterManager::on_update(float delta)
{
    player->on_update(delta);
    enemy->on_update(delta);
}

void CharacterManager::on_render(const Camera &camera)
{
    enemy->on_render(camera);
    BulletTimeManager::instance().post_process(camera);
    player->on_render(camera);
}
