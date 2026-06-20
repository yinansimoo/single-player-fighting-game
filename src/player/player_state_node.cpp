#include "player.h"
#include "character_manager.h"
#include "player_state_node.h"
#include "resource_manager.h"
#include "game.h"

PlayerAttackState::PlayerAttackState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.3f);
    timer.set_on_timeout([&]()
                         { Player *player = (Player*)CharacterManager::instance().get_player(); 
                            player->set_attacking(false); });
}

void PlayerAttackState::on_enter()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->set_animation("attack");

    player->get_hit_box()->set_enable(true);
    player->set_attacking(true);
    update_hit_box_position();
    player->on_attack();
    timer.restart();

    int ran = rand() % 3 + 1;
    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_attack_" + std::to_string(ran)), 0);
}

void PlayerAttackState::on_update(float delta)
{
    timer.update(delta);
    update_hit_box_position();

    Player *player = (Player *)CharacterManager::instance().get_player();

    if (player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if (!player->get_attacking())
    {
        if (player->get_velocity().y > 0)
        {
            player->switch_state("fall");
        }
        else if (player->get_move_axis() == 0)
        {
            player->switch_state("idle");
        }
        else if (player->is_on_floor() && player->get_move_axis() != 0)
        {
            player->switch_state("run");
        }
    }
}

void PlayerAttackState::on_exit()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->get_hit_box()->set_enable(false);
    player->set_attacking(false);
}

void PlayerAttackState::update_hit_box_position()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    Vector2 player_pos = player->get_position();
    CollisionBox *hit_box = player->get_hit_box();
    const Vector2 &size_hit_box = hit_box->get_size();
    Vector2 pos_hit_box;
    switch (player->get_attack_dir())
    {
    case Player::AttackDir::UP:
        pos_hit_box = {player_pos.x, player_pos.y - size_hit_box.y / 2};
        break;
    case Player::AttackDir::DOWN:
        pos_hit_box = {player_pos.x, player_pos.y + size_hit_box.y / 2};
        break;
    case Player::AttackDir::LEFT:
        pos_hit_box = {player_pos.x - size_hit_box.x / 2, player_pos.y};
        break;
    case Player::AttackDir::RIGHT:
        pos_hit_box = {player_pos.x + size_hit_box.x / 2, player_pos.y};
    default:
        break;
    }
    hit_box->set_position(pos_hit_box);
}

PlayerDeadState::PlayerDeadState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(2.0f);
    timer.set_on_timeout([&]()
                         {
        Player *player = (Player *)CharacterManager::instance().get_player();
        Vector2 player_position = player->get_position();
        float player_height = player->get_logic_height();
        Vector2 set_position = {player_position.x,player_position.y - (int)player_height/2};
        std::string text = "不行，还不能倒下......";
        Game::initstate().draw_text(text.c_str(),set_position);

        std::string boxtext = "请继续加油";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "挑战失败",
            boxtext.c_str(),
            NULL);
        exit(0); });
}

void PlayerDeadState::on_enter()
{
    CharacterManager::instance().get_player()->set_animation("dead");

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_dead"), 0);
}

void PlayerDeadState::on_update(float delta)
{
    timer.update(delta);
}

void PlayerFallState::on_enter()
{
    CharacterManager::instance().get_player()->set_animation("fall");
}

void PlayerFallState::on_update(float delta)
{
    Player *player = (Player *)CharacterManager::instance().get_player();

    if (player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if (player->is_on_floor())
    {
        player->switch_state("idle");
        player->on_land();

        Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_land"), 0);
    }
    else if (player->can_attack())
    {
        player->switch_state("attack");
    }
}

void PlayerIdleState::on_enter()
{
    CharacterManager::instance().get_player()->set_animation("idle");
}

void PlayerIdleState::on_update(float delta)
{
    Player *player = (Player *)CharacterManager::instance().get_player();

    if (player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if (player->can_attack())
    {
        player->switch_state("attack");
    }
    else if (player->can_jump())
    {
        player->switch_state("jump");
    }
    else if (player->can_roll())
    {
        player->switch_state("roll");
    }
    else if (player->get_velocity().y > 0)
    {
        player->switch_state("fall");
    }
    else if (player->is_on_floor() && player->get_move_axis() != 0)
    {
        player->switch_state("run");
    }
}

void PlayerJumpState::on_enter()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->set_animation("jump");
    player->on_jump();

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_jump"), 0);
}

void PlayerJumpState::on_update(float delta)
{
    Player *player = (Player *)CharacterManager::instance().get_player();

    if (player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if (player->can_attack())
    {
        player->switch_state("attack");
    }
    else if (player->get_velocity().y > 0)
    {
        player->switch_state("fall");
    }
}

PlayerRollState::PlayerRollState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.35f);
    timer.set_on_timeout([&]()
                         { 
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->set_rolling(false); });
}

void PlayerRollState::on_enter()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->set_animation("roll");
    player->get_hurt_box()->set_enable(false);
    player->set_rolling(true);
    player->on_roll();
    timer.restart();

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_roll"), 0);
}

void PlayerRollState::on_update(float delta)
{
    timer.update(delta);

    Player *player = (Player *)CharacterManager::instance().get_player();

    if (!player->get_rolling())
    {
        if (player->can_jump())
        {
            player->switch_state("jump");
        }
        else if (player->get_move_axis() != 0)
        {
            player->switch_state("run");
        }
        else
        {
            player->switch_state("idle");
        }
    }
}

void PlayerRollState::on_exit()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->get_hurt_box()->set_enable(true);
}

void PlayerRunState::on_enter()
{
    Player *player = (Player *)CharacterManager::instance().get_player();
    player->set_animation("run");

    Mix_PlayChannel(0, ResourceManage::instance().find_chunk("player_run"), -1);
}

void PlayerRunState::on_update(float delta)
{
    Player *player = (Player *)CharacterManager::instance().get_player();

    if (player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if (player->can_attack())
    {
        player->switch_state("attack");
    }
    else if (player->can_jump())
    {
        player->switch_state("jump");
    }
    else if (player->get_move_axis() == 0)
    {
        player->switch_state("idle");
    }
    else if (player->can_roll())
    {
        player->switch_state("roll");
    }
}

void PlayerRunState::on_exit()
{
    Mix_HaltChannel(0);
}
