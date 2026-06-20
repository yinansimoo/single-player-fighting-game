#include "player.h"
#include "resource_manager.h"
#include "player_state_node.h"
#include "bullet_time_manager.h"

Player::Player()
{
    //  初始朝向，位置，逻辑高度
    {
        is_facing_left = false;
        position = {250, 200};
        SDL_Texture *text = ResourceManage::instance().find_texture("player_idle_right");
        if (text != nullptr)
        {
            int w, h;
            SDL_QueryTexture(text, nullptr, nullptr, &w, &h);
            logic_height = (float)h;
        }
    }

    //  碰撞箱
    {
        hit_box->set_size({150, 150});
        hurt_box->set_size({40, 80});

        hit_box->set_layer_src(CollisionLayer::NONE);
        hit_box->set_layer_dst(CollisionLayer::ENEMY);

        hurt_box->set_layer_src(CollisionLayer::PLAYER);
        hurt_box->set_layer_dst(CollisionLayer::NONE);

        hit_box->set_enable(false);
        hurt_box->set_on_collide([&]()
                                 { decrease_hp(); });
    }

    //  定时器
    {
        timer_roll_cd.set_wait_time(CD_ROLL);
        timer_roll_cd.set_on_shot(true);
        timer_roll_cd.set_on_timeout([&]()
                                     { is_roll_cd_comp = true; });

        timer_attack_cd.set_wait_time(CD_ATTACK);
        timer_attack_cd.set_on_shot(true);
        timer_attack_cd.set_on_timeout([&]()
                                       { is_attack_cd_comp = true; });
    }

    //  纹理，特效
    {
        init_texture("attack", 0.05f, false, "player_attack", 5);
        init_texture("dead", 0.1f, false, "player_dead", 6);
        init_texture("fall", 0.15f, true, "player_fall", 5);
        init_texture("idle", 0.15f, true, "player_idle", 5);
        init_texture("jump", 0.15f, false, "player_jump", 5);
        init_texture("roll", 0.05f, false, "player_roll", 7);
        init_texture("run", 0.075f, true, "player_run", 10);

        init_animation(&animation_alash_up, 0.07f, false, "player_vfx_attack_up", 5);
        init_animation(&animation_alash_down, 0.07f, false, "player_vfx_attack_down", 5);
        init_animation(&animation_alash_left, 0.07f, false, "player_vfx_attack_left", 5);
        init_animation(&animation_alash_right, 0.07f, false, "player_vfx_attack_right", 5);
        init_animation(&animation_jump_vfx, 0.05f, false, "player_vfx_jump", 5);
        init_animation(&animation_land_vfx, 0.1f, false, "player_vfx_land", 5);

        animation_jump_vfx.set_on_finished([&]()
                                           { is_jump_vfx_visible = false; });
        animation_land_vfx.set_on_finished([&]()
                                           { is_land_vfx_visible = false; });
    }

    //  状态机
    {
        state_machine.register_state("attack", new PlayerAttackState());
        state_machine.register_state("dead", new PlayerDeadState());
        state_machine.register_state("fall", new PlayerFallState());
        state_machine.register_state("idle", new PlayerIdleState());
        state_machine.register_state("jump", new PlayerJumpState());
        state_machine.register_state("roll", new PlayerRollState());
        state_machine.register_state("run", new PlayerRunState());

        state_machine.set_entry("idle");
    }
}

void Player::init_texture(const std::string &id, float interval, bool loop, const std::string &resource_id, int num)
{
    AnimationGroup &group = animation_pool[id];

    Animation &left = group.left;
    left.set_interval(interval);
    left.set_is_loop(loop);
    left.add_frame(ResourceManage::instance().find_texture(resource_id + "_left"), num);

    Animation &right = group.right;
    right.set_interval(interval);
    right.set_is_loop(loop);
    right.add_frame(ResourceManage::instance().find_texture(resource_id + "_right"), num);
}

void Player::init_animation(Animation *animation, float interval, bool loop, const std::string &resource_id, int num)
{
    animation->set_interval(interval);
    animation->set_is_loop(loop);
    animation->add_frame(ResourceManage::instance().find_texture(resource_id), num);
}

//  按键
void Player::on_input(const SDL_Event &event)
{
    if (hp <= 0)
    {
        return;
    }

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
            is_left_key_down = true;
            break;
        case SDLK_d:
            is_right_key_down = true;
            break;
        case SDLK_w:
        case SDLK_SPACE:
            is_jump_key_down = true;
            break;
        case SDLK_s:
            is_roll_key_down = true;
            break;
        default:
            break;
        }
    }

    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
            is_left_key_down = false;
            break;
        case SDLK_d:
            is_right_key_down = false;
            break;
        case SDLK_w:
        case SDLK_SPACE:
            is_jump_key_down = false;
            break;
        case SDLK_s:
            is_roll_key_down = false;
            break;
        default:
            break;
        }
    }

    //  鼠标按下
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        //  左键
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            is_attack_key_down = true;
            //  根据鼠标坐标更新攻击朝向
            int x, y;
            SDL_GetMouseState(&x, &y);
            update_attack_dir(x, y);
        }

        //  右键
        //  子弹时间
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            Mix_PlayChannel(1, ResourceManage::instance().find_chunk("bullet_time"), 0);
            BulletTimeManager::instance().set_status(BulletTimeManager::Status::ENTERING);
        }
    }

    // 鼠标松
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            is_attack_key_down = false;
        }

        //  右键
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            BulletTimeManager::instance().set_status(BulletTimeManager::Status::EXITING);
        }
    }
}

void Player::on_update(float delta)
{
    if (hp > 0 && !is_rolling)
    {
        velocity.x = get_move_axis() * SPEED_RUN;
    }

    int axis = get_move_axis();
    if (axis != 0)
    {
        is_facing_left = (axis < 0);
    }

    timer_roll_cd.update(delta);
    timer_attack_cd.update(delta);

    animation_jump_vfx.on_update(delta);
    animation_land_vfx.on_update(delta);

    if (is_attacking)
    {
        current_slash_animation->set_position(position);
        current_slash_animation->on_update(delta);
    }

    Character::on_update(delta);
}

void Player::on_render(const Camera &camera) const
{
    if (is_jump_vfx_visible)
    {
        animation_jump_vfx.on_render(camera);
    }
    if (is_land_vfx_visible)
    {
        animation_land_vfx.on_render(camera);
    }

    Character::on_render(camera);

    if (is_attacking)
    {
        current_slash_animation->on_render(camera);
    }
}

void Player::on_hurt()
{
    Mix_PlayChannel(-1, ResourceManage::instance().find_chunk("player_hurt"), 0);
}

void Player::set_rolling(bool flag)
{
    this->is_rolling = flag;
}

bool Player::get_rolling() const
{
    return is_rolling;
}

bool Player::can_roll() const
{
    return is_roll_cd_comp && !is_rolling && is_roll_key_down;
}

void Player::set_attacking(bool flag)
{
    this->is_attacking = flag;
}

bool Player::get_attacking() const
{
    return is_attacking;
}

bool Player::can_attack() const
{
    return is_attack_cd_comp && !is_attacking && is_attack_key_down;
}

bool Player::can_jump() const
{
    return is_on_floor() && is_jump_key_down;
}

int Player::get_move_axis() const
{
    return is_right_key_down - is_left_key_down;
}

Player::AttackDir Player::get_attack_dir() const
{
    return attack_dir;
}

void Player::on_jump()
{
    velocity.y -= SPEED_JUMP;
    is_jump_vfx_visible = true;
    animation_jump_vfx.set_position({position.x, position.y});
    animation_jump_vfx.reset();
}

void Player::on_land()
{
    is_land_vfx_visible = true;
    animation_land_vfx.set_position({position.x, position.y});
    animation_land_vfx.reset();
}

void Player::on_roll()
{
    timer_roll_cd.restart();
    is_roll_cd_comp = false;
    velocity.x = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;
}

void Player::on_attack()
{
    timer_attack_cd.restart();
    is_attack_cd_comp = false;
    switch (attack_dir)
    {
    case AttackDir::UP:
        current_slash_animation = &animation_alash_up;
        break;
    case AttackDir::DOWN:
        current_slash_animation = &animation_alash_down;
        break;
    case AttackDir::LEFT:
        current_slash_animation = &animation_alash_left;
        break;
    case AttackDir::RIGHT:
        current_slash_animation = &animation_alash_right;
        break;
    default:
        break;
    }
    current_slash_animation->set_position(position);
    current_slash_animation->reset();
}

void Player::update_attack_dir(int x, int y)
{
    static constexpr float PI = 3.141592654f;
    const float dx = x - position.x;
    const float dy = y - position.y;
    const float angle = std::atan2(dy, dx);

    if (angle >= -PI / 4 && angle < PI / 4)
        attack_dir = AttackDir::RIGHT;
    else if (angle >= PI / 4 && angle < 3 * PI / 4)
        attack_dir = AttackDir::DOWN;
    else if (angle >= 3 * PI / 4 || angle < -3 * PI / 4)
        attack_dir = AttackDir::LEFT;
    else
        attack_dir = AttackDir::UP;
}
