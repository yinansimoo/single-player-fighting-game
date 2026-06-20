#include "enemy.h"
#include "player.h"
#include "character_manager.h"
#include "enemy_state_node.h"
#include "resource_manager.h"

EnemyAimState::EnemyAimState()
{
    timer.set_wait_time(0.5f);
    timer.set_on_shot(true);
    timer.set_on_timeout([&]()
                         {
        Enemy* enemy = (Enemy*)CharacterManager::instance().get_enemy();
        enemy->set_gravity_enable(true);
        enemy->switch_state("dash_in_air"); });
}

void EnemyAimState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    enemy->set_animation("aim");
    enemy->set_gravity_enable(false);
    enemy->set_velocity({0, 0});
    timer.restart();
}

void EnemyAimState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
}

void EnemyDashInAirState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    const Player *player = (Player *)CharacterManager::instance().get_player();

    enemy->set_animation("dash_in_air");
    Vector2 pos_target = {player->get_position().x, player->get_floor_y()};
    enemy->set_velocity((pos_target - enemy->get_position()).normalize() * SPEED_DASH);
    enemy->set_dashing_in_air(true);
    enemy->set_gravity_enable(false);
    enemy->on_dash();

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("enemy_dash"), 0);
}

void EnemyDashInAirState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (enemy->is_on_floor())
    {
        enemy->switch_state("idle");
    }
}

void EnemyDashInAirState::on_exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_dashing_in_air(false);
    enemy->set_gravity_enable(true);
}

EnemyDashOnFloorState::EnemyDashOnFloorState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.5f);
    timer.set_on_timeout([&]()
                         {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

        enemy->set_dashing_on_floor(false); });
}

void EnemyDashOnFloorState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("dash_on_floor");
    enemy->set_velocity({enemy->get_facing_left() ? -SPEED_DASH : SPEED_DASH, 0});
    enemy->set_dashing_on_floor(true);
    enemy->on_dash();
    timer.restart();

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("enemy_dash"), 0);
}

void EnemyDashOnFloorState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (!enemy->get_dashing_on_floor())
    {
        enemy->switch_state("idle");
    }
}

void EnemyDeadState::on_enter()
{
    std::string boxtext = "恭喜您成功取得胜利！";

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "挑战成功",
        boxtext.c_str(),
        NULL);
    exit(0);
}

void EnemyDeadState::on_update(float delta)
{
}

void EnemyFallState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("fall");
}

void EnemyFallState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (enemy->is_on_floor())
    {
        enemy->switch_state("idle");
    }
}

EnemyIdleState::EnemyIdleState()
{
    timer.set_on_shot(true);
    timer.set_on_timeout([&]()
                         {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

        int rand_num = rand()% 101;
    
        if(enemy->get_hp() > 5)
        {
            if(rand_num <= 25)
            {
                if(!enemy->is_on_floor())
                {
                    enemy->switch_state("fall");
                }
                else
                {
                    enemy->switch_state("jump");
                }
            }
            else if(rand_num <= 50)
            {
                if(!enemy->is_on_floor())
                {
                    enemy->switch_state("fall");
                }
                else
                {
                    enemy->switch_state("run");
                }
            }
            else if(rand_num <= 80)
            {
                enemy->switch_state("squat");
            }
            else if(rand_num <= 90)
            {
                enemy->switch_state("throw_silk");
            }
            else 
            {
                enemy->switch_state("throw_sword");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
            }
        }
        else
        {
            if(rand_num <= 25)
            {
                if(!enemy->is_on_floor())
                {
                    enemy->switch_state("fall");
                }
                else
                {
                    enemy->switch_state("jump");
                }
            }
            else if(rand_num<= 60)
            {
                enemy->switch_state("throw_sword");
            }
            else if(rand_num <= 70)
            {
                enemy->switch_state("throw_silk");
            }
            else if(rand_num <= 90)
            {
                enemy->switch_state("throw_barb");
            }
            else
            {
                enemy->switch_state("squat");
            }
        } });
}

void EnemyIdleState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    enemy->set_animation("idle");

    enemy->set_velocity({0, 0});

    float wait = 0;
    if (enemy->get_hp() > 5)
    {
        wait = (rand() % 3) * 0.25f;
    }
    else
    {
        wait = (rand() % 2) * 0.25f;
    }

    timer.set_wait_time(wait);
    timer.restart();
}

void EnemyIdleState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (enemy->get_velocity().y > 0)
    {
        enemy->switch_state("fall");
    }
}

void EnemyIdleState::on_exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    enemy->set_facing_left(enemy->get_position().x > CharacterManager::instance().get_player()->get_position().x);
}

void EnemyJumpState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    enemy->set_animation("jump");
    enemy->set_velocity({0, -SPEED_JUMP});
}

void EnemyJumpState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (enemy->get_velocity().y > 0)
    {
        int rand_num = rand() % 101;
        if (enemy->get_hp() > 5)
        {
            if (rand_num <= 50)
            {
                enemy->switch_state("aim");
            }
            else if (rand_num <= 80)
            {
                enemy->switch_state("fall");
            }
            else
            {
                enemy->switch_state("throw_silk");
            }
        }
        else
        {
            if (rand_num <= 50)
            {
                enemy->switch_state("throw_silk");
            }
            else if (rand_num <= 80)
            {
                enemy->switch_state("fall");
            }
            else
            {
                enemy->switch_state("aim");
            }
        }
    }
}

void EnemyRunState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("run");
    Mix_PlayChannel(1, ResourceManage::instance().find_chunk("enemy_run"), -1);
}

void EnemyRunState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    const Vector2 &pos_enemy = enemy->get_position();
    const Vector2 &pos_player = CharacterManager::instance().get_player()->get_position();
    enemy->set_velocity({pos_enemy.x < pos_player.x ? SPEED_RUN : -SPEED_RUN, 0});

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if (abs(pos_enemy.x - pos_player.x) <= MIN_DIS)
    {
        int rand_num = rand() % 101;
        if (enemy->get_hp() > 5)
        {
            if (rand_num <= 75)
            {
                enemy->switch_state("squat");
            }
            else
            {
                enemy->switch_state("throw_silk");
            }
        }
        else
        {
            if (rand_num <= 75)
            {
                enemy->switch_state("throw_silk");
            }
            else
            {
                enemy->switch_state("squat");
            }
        }
        Mix_HaltChannel(1);
    }
}

void EnemyRunState::on_exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_velocity({0, 0});
}

EnemySquatState::EnemySquatState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.5f);
    timer.set_on_timeout([&]()
                         {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
        enemy->switch_state("dash_on_floor"); });
}

void EnemySquatState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("squat");
    enemy->set_facing_left(enemy->get_position().x > CharacterManager::instance().get_player()->get_position().x);
    timer.restart();
}

void EnemySquatState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
}

EnemyThrowBarbState::EnemyThrowBarbState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.8f);
    timer.set_on_timeout([&]()
                         {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
        enemy->throw_barbs();
        enemy->switch_state("idle"); });
}

void EnemyThrowBarbState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("throw_barb");
    timer.restart();
    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("enemy_throw_barbs"), 0);
}

void EnemyThrowBarbState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
}

EnemyThrowSilkState::EnemyThrowSilkState()
{
    timer.set_on_shot(true);
    timer.set_wait_time(0.9f);
    timer.set_on_timeout([&]()
                         {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
        enemy->set_gravity_enable(true);
        enemy->set_throwing_silk(false); 
        if(!enemy->is_on_floor() && enemy->get_hp() > 5 && (rand()% 101) <= 25)
        {
            enemy->switch_state("aim");
        } 
        else if(!enemy->is_on_floor())
        {
            enemy->switch_state("fall");
        }
        else
        {
            enemy->switch_state("idle");
        } });
}

void EnemyThrowSilkState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    enemy->set_animation("throw_silk");
    enemy->set_gravity_enable(false);
    enemy->set_throwing_silk(true);
    enemy->set_velocity({0, 0});
    enemy->on_throw_silk();
    timer.restart();

    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("enemy_throw_silk"), 0);
}

void EnemyThrowSilkState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
}

EnemyThrowSwordState::EnemyThrowSwordState()
{
    timer_throw.set_wait_time(0.65f);
    timer_throw.set_on_shot(true);
    timer_throw.set_on_timeout([&]()
                               {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
        enemy->throw_sword();

        Mix_PlayChannel(-1,ResourceManage::instance().find_chunk("enemy_throw_sword"),0); });

    timer_switch.set_wait_time(1.0f);
    timer_switch.set_on_shot(true);
    timer_switch.set_on_timeout([&]()
                                {
        Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

        int rand_num = rand()%101;
        if(enemy->get_hp()>5)
        {
            if(rand_num <= 50)
            {
                enemy->switch_state("squat");
            }
            else if(rand_num <= 80)
            {
                enemy->switch_state("jump");
            }
            else
            {
                enemy->switch_state("idle");
            }
        }
        else
        {
            if(rand_num <= 50)
            {
                enemy->switch_state("jump");
            }
            else if(rand_num <= 80)
            {
                enemy->switch_state("throw_silk");
            }
            else
            {
                enemy->switch_state("idle");
            }
        } });
}

void EnemyThrowSwordState::on_enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();
    enemy->set_animation("throw_sword");
    timer_throw.restart();
    timer_switch.restart();
}

void EnemyThrowSwordState::on_update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::instance().get_enemy();

    timer_throw.update(delta);
    timer_switch.update(delta);

    if (enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
}
