#include "character.h"
#include "collision_manager.h"
#include "game.h"

#include "SDL.h"

Character::Character()
{
    hit_box = CollisionManager::initstate().create_collision_box();
    hurt_box = CollisionManager::initstate().create_collision_box();

    timer_invulnerable_state.set_wait_time(1.0f);
    timer_invulnerable_state.set_on_shot(true);
    timer_invulnerable_state.set_on_timeout([&]()
                                            { is_invulnerable = false; });

    timer_invulnerable_blink.set_wait_time(0.075f);
    timer_invulnerable_blink.set_on_shot(false);
    timer_invulnerable_blink.set_on_timeout([&]()
                                            { is_blink_invisible = !is_blink_invisible; });
}

Character::~Character()
{
    CollisionManager::initstate().destroy_collision_box(hit_box);
    CollisionManager::initstate().destroy_collision_box(hurt_box);
}

void Character::decrease_hp()
{
    if (is_invulnerable)
    {
        return;
    }

    hp -= 1;
    if (hp > 0)
    {
        make_invulnerable();
    }
    on_hurt();
}

int Character::get_hp() const
{
    return this->hp;
}

void Character::set_position(const Vector2 &position)
{
    this->position = position;
}

const Vector2 &Character::get_position() const
{
    return this->position;
}

void Character::set_velocity(const Vector2 &velocity)
{
    this->velocity = velocity;
}

const Vector2 &Character::get_velocity() const
{
    return this->velocity;
}

void Character::set_gravity_enable(bool flag)
{
    this->enable_gravity = flag;
}

CollisionBox *Character::get_hit_box()
{
    return this->hit_box;
}

CollisionBox *Character::get_hurt_box()
{
    return this->hurt_box;
}

bool Character::is_on_floor() const
{
    return (position.y + logic_height / 2) >= FLOOR_Y;
}

float Character::get_floor_y() const
{
    return this->FLOOR_Y;
}

void Character::make_invulnerable()
{
    is_invulnerable = true;
    timer_invulnerable_state.restart();
}

float Character::get_logic_height()
{
    return logic_height;
}

void Character::on_input(const SDL_Event &event)
{
}

void Character::on_update(float delta)
{
    state_machine.on_update(delta);

    if (hp <= 0)
    {
        velocity.x = 0;
    }

    if (enable_gravity)
    {
        velocity.y += GRAVITY * delta;
    }

    position += velocity * delta;

    if (position.y >= FLOOR_Y - logic_height / 2)
    {
        position.y = FLOOR_Y - logic_height / 2;
        velocity.y = 0;
    }

    if (position.x <= 0)
    {
        position.x = 0;
    }
    if (position.x >= Game::initstate().get_windowwidth())
    {
        position.x = Game::initstate().get_windowwidth();
    }

    hurt_box->set_position(position);

    timer_invulnerable_state.update(delta);

    if (is_invulnerable)
    {
        timer_invulnerable_blink.update(delta);
    }

    if (current_animation == nullptr)
    {
        return;
    }

    Animation &animation = is_facing_left ? current_animation->left : current_animation->right;
    animation.on_update(delta);
    animation.set_position(position);
}

void Character::on_render(const Camera &camera) const
{
    if (current_animation == nullptr || (is_invulnerable && is_blink_invisible))
    {
        return;
    }

    (is_facing_left ? current_animation->left : current_animation->right).on_render(camera);
}

void Character::on_hurt()
{
}

void Character::switch_state(const std::string &id)
{
    state_machine.switch_to(id);
}

void Character::set_animation(const std::string &id)
{
    auto it = animation_pool.find(id);
    if (it == animation_pool.end())
    {
        std::string error = "set_animation失败：" + id;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "角色放置动画错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }

    current_animation = &it->second;
    current_animation->left.reset();
    current_animation->right.reset();
}
