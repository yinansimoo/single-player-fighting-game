#include "enemy.h"
#include "enemy_state_node.h"
#include "character_manager.h"
#include "resource_manager.h"
#include "collision_manager.h"
#include "game.h"

Enemy::Enemy()
{
    {
        is_facing_left = true;
        position = {1050, 200};
        SDL_Texture *text = ResourceManage::instance().find_atlas("enemy_idle_left")->get_texture(0);
        if (text != nullptr)
        {
            int w, h;
            SDL_QueryTexture(text, nullptr, nullptr, &w, &h);
            logic_height = (float)h;
        }
    }

    //  碰撞箱
    {
        hit_box->set_size({50, 80});
        hurt_box->set_size({100, 180});

        hit_box->set_layer_src(CollisionLayer::NONE);
        hit_box->set_layer_dst(CollisionLayer::PLAYER);

        hurt_box->set_layer_src(CollisionLayer::ENEMY);
        hurt_box->set_layer_dst(CollisionLayer::NONE);

        hurt_box->set_on_collide([&]()
                                 { decrease_hp(); });

        box_silk = CollisionManager::initstate().create_collision_box();
        box_silk->set_size({225, 225});
        box_silk->set_layer_src(CollisionLayer::NONE);
        box_silk->set_layer_dst(CollisionLayer::PLAYER);
        box_silk->set_enable(false);
    }

    // 动画
    {
        init_animationGroup("aim", 0.05f, false, "enemy_aim");
        init_animationGroup("dash_in_air", 0.05f, true, "enemy_dash_in_air");
        init_animationGroup("dash_on_floor", 0.05f, true, "enemy_dash_on_floor");
        init_animationGroup("fall", 0.1f, true, "enemy_fall");
        init_animationGroup("idle", 0.1f, true, "enemy_idle");
        init_animationGroup("jump", 0.1f, true, "enemy_jump");
        init_animationGroup("run", 0.05f, true, "enemy_run");
        init_animationGroup("squat", 0.05f, true, "enemy_squat");
        init_animationGroup("throw_barb", 0.1f, false, "enemy_throw_barb");
        init_animationGroup("throw_silk", 0.1f, true, "enemy_throw_silk");
        init_animationGroup("throw_sword", 0.1f, false, "enemy_throw_sword");

        init_animation(&animation_silk, 0.1f, false, "silk");
        init_animation(&animation_dash_in_air_vfx.left, 0.1f, false, "enemy_vfx_dash_in_air_left");
        init_animation(&animation_dash_in_air_vfx.right, 0.1f, false, "enemy_vfx_dash_in_air_right");
        init_animation(&animation_dash_on_floor_vfx.left, 0.1f, false, "enemy_vfx_dash_on_floor_left");
        init_animation(&animation_dash_on_floor_vfx.right, 0.1f, false, "enemy_vfx_dash_on_floor_right");
    }

    //  状态机
    {
        state_machine.register_state("aim", new EnemyAimState());
        state_machine.register_state("dash_in_air", new EnemyDashInAirState());
        state_machine.register_state("dash_on_floor", new EnemyDashOnFloorState());
        state_machine.register_state("dead", new EnemyDeadState());
        state_machine.register_state("fall", new EnemyFallState());
        state_machine.register_state("idle", new EnemyIdleState());
        state_machine.register_state("jump", new EnemyJumpState());
        state_machine.register_state("run", new EnemyRunState());
        state_machine.register_state("squat", new EnemySquatState());
        state_machine.register_state("throw_barb", new EnemyThrowBarbState());
        state_machine.register_state("throw_silk", new EnemyThrowSilkState());
        state_machine.register_state("throw_sword", new EnemyThrowSwordState());

        state_machine.set_entry("idle");
    }
}

Enemy::~Enemy()
{
    CollisionManager::initstate().destroy_collision_box(box_silk);
}

void Enemy::init_animationGroup(const std::string &id, float interval, bool loop, const std::string &resource_id)
{
    AnimationGroup &group = animation_pool[id];

    Animation &left = group.left;
    left.set_interval(interval);
    left.set_is_loop(loop);
    left.add_frame(ResourceManage::instance().find_atlas(resource_id + "_left"));

    Animation &right = group.right;
    right.set_interval(interval);
    right.set_is_loop(loop);
    right.add_frame(ResourceManage::instance().find_atlas(resource_id + "_right"));
}

void Enemy::init_animation(Animation *animation, float interval, bool loop, const std::string &resource_id)
{
    animation->set_interval(interval);
    animation->set_is_loop(loop);
    animation->add_frame(ResourceManage::instance().find_atlas(resource_id));
}

void Enemy::on_update(float delta)
{
    if (velocity.x >= 0.0001f)
    {
        is_facing_left = (velocity.x < 0);
    }

    Character::on_update(delta);

    hit_box->set_position(position);

    if (is_throwing_silk)
    {
        box_silk->set_position(position);
        animation_silk.set_position(position);
        animation_silk.on_update(delta);
    }

    if (is_dashing_in_air || is_dashing_on_floor)
    {
        current_dash_animation->set_position(is_dashing_in_air ? position : position - Vector2{0, get_logic_height() / 2});
        current_dash_animation->on_update(delta);
    }

    for (Barb *barb : barb_list)
    {
        barb->on_update(delta);
    }

    for (Sword *sword : sword_list)
    {
        sword->on_update(delta);
    }

    barb_list.erase(std::remove_if(barb_list.begin(), barb_list.end(), [](Barb *barb)
                                   {
    bool can_remove = !barb->check_valid();
    if(can_remove)
    {
        delete barb;
    }
    return can_remove; }),
                    barb_list.end());

    sword_list.erase(std::remove_if(sword_list.begin(), sword_list.end(), [](Sword *sword)
                                    {
    bool can_remove = !sword->check_valid();
    if(can_remove)
    {
        delete sword;
    }
    return can_remove; }),
                     sword_list.end());
}

void Enemy::on_render(const Camera &camera) const
{
    for (Barb *barb : barb_list)
    {
        barb->on_render(camera);
    }
    for (Sword *sword : sword_list)
    {
        sword->on_render(camera);
    }

    Character::on_render(camera);

    if (is_throwing_silk)
    {
        animation_silk.on_render(camera);
    }

    if (is_dashing_in_air || is_dashing_on_floor)
    {
        current_dash_animation->on_render(camera);
    }
}

void Enemy::on_hurt()
{
    int ran = rand() % 3 + 1;
    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("enemy_hurt_" + std::to_string(ran)), 0);
}

void Enemy::set_facing_left(bool flag)
{
    this->is_facing_left = flag;
}

bool Enemy::get_facing_left() const
{
    return this->is_facing_left;
}

void Enemy::set_dashing_in_air(bool flag)
{
    this->is_dashing_in_air = flag;
}

bool Enemy::get_dashing_in_air() const
{
    return this->is_dashing_in_air;
}

void Enemy::set_dashing_on_floor(bool flag)
{
    this->is_dashing_on_floor = flag;
}

bool Enemy::get_dashing_on_floor() const
{
    return is_dashing_on_floor;
}

void Enemy::set_throwing_silk(bool flag)
{
    this->is_throwing_silk = flag;
}

bool Enemy::get_throwing_silk() const
{
    return this->is_throwing_silk;
}

void Enemy::throw_barbs()
{
    int num_new_barb = rand() % 4 + 3;
    if (barb_list.size() >= 10)
    {
        num_new_barb = 1;
    }
    int width_grid = Game::initstate().get_windowwidth() / num_new_barb;

    for (int i = 0; i < num_new_barb; i++)
    {
        Barb *barb = new Barb();
        int rand_x = rand() % (width_grid + 1) + (width_grid * i);
        int rand_y = rand() % 251 + 250;
        barb->set_position({(float)rand_x, (float)rand_y});
        barb_list.push_back(barb);
    }
}

void Enemy::throw_sword()
{
    Sword *sword = new Sword(position, is_facing_left);
    sword_list.push_back(sword);
}

void Enemy::on_dash()
{
    if (is_dashing_in_air)
    {
        current_dash_animation = velocity.x < 0 ? &animation_dash_in_air_vfx.left : &animation_dash_in_air_vfx.right;
    }
    else
    {
        current_dash_animation = velocity.x < 0 ? &animation_dash_on_floor_vfx.left : &animation_dash_on_floor_vfx.right;
    }

    current_dash_animation->reset();
}

void Enemy::on_throw_silk()
{
    animation_silk.reset();
}
