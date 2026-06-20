#include "barb.h"
#include "resource_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "game.h"

Barb::Barb()
{
    diff_period = rand() % 7;

    animation_loose.set_interval(0.15f);
    animation_loose.set_is_loop(true);
    animation_loose.add_frame(ResourceManage::instance().find_atlas("barb_loose"));

    Animation_break.set_interval(0.1f);
    Animation_break.set_is_loop(false);
    Animation_break.add_frame(ResourceManage::instance().find_atlas("barb_break"));
    Animation_break.set_on_finished([&]()
                                    { is_valid = false; });

    box = CollisionManager::initstate().create_collision_box();
    box->set_layer_dst(CollisionLayer::PLAYER);
    box->set_layer_src(CollisionLayer::ENEMY);
    box->set_size({20, 20});
    box->set_on_collide([&]()
                        { on_break(); });

    timer_idle.set_on_shot(true);
    timer_idle.set_wait_time((float)(rand() % 8 + 3));
    timer_idle.set_on_timeout([&]()
                              { 
                                if(state == State::IDLE)
                            {
                                state = State::AIM; 
                                base_position = current_position;
                            } });

    timer_aim.set_on_shot(true);
    timer_aim.set_wait_time(0.75);
    timer_aim.set_on_timeout([&]()
                             {
        if(state == State::AIM)
        {
            state = State::DASH;
            const Vector2& pos_player = CharacterManager::instance().get_player()->get_position();
            velocity = (pos_player - current_position).normalize() * SPEED_DASH;
        } });
}

Barb::~Barb()
{
    CollisionManager::initstate().destroy_collision_box(box);
}

void Barb::on_update(float delta)
{
    if (state == State::IDLE)
    {
        timer_idle.update(delta);
    }

    if (state == State::AIM)
    {
        timer_aim.update(delta);
    }

    total_delta_time += delta;
    switch (state)
    {
    case State::IDLE:
        current_position.y = base_position.y + sin(total_delta_time * 2 + diff_period) * 30;
        break;
    case State::AIM:
        current_position.x = base_position.x + rand() % 21 - 10;
        break;
    case State::DASH:
        current_position += velocity * delta;
        if (current_position.y >= CharacterManager::instance().get_player()->get_floor_y())
        {
            on_break();
        }
        if (current_position.y <= 0 || current_position.x >= Game::initstate().get_windowwidth() || current_position.x <= -20)
        {
            is_valid = false;
        }
        break;
    default:
        break;
    }
    box->set_position(current_position);

    current_animation = (state == State::BREAK) ? &Animation_break : &animation_loose;
    current_animation->set_position(current_position);
    current_animation->on_update(delta);
}

void Barb::on_render(const Camera &camera) const
{
    current_animation->on_render(camera);
}

void Barb::set_position(const Vector2 &position)
{
    current_position = position;
    base_position = position;
}

bool Barb::check_valid() const
{
    return is_valid;
}

void Barb::on_break()
{
    if (state == State::BREAK)
    {
        return;
    }

    state = State::BREAK;
    box->set_enable(false);
    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("barb_break"), 0);
}