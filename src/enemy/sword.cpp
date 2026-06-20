#include "sword.h"
#include "resource_manager.h"
#include "collision_manager.h"
#include "game.h"

Sword::Sword(const Vector2 &position, bool move_left)
{
    animation.set_interval(0.1f);
    animation.set_is_loop(true);
    animation.add_frame(ResourceManage::instance().find_atlas(move_left ? "sword_left" : "sword_right"));

    hit_box = CollisionManager::initstate().create_collision_box();
    hit_box->set_layer_src(CollisionLayer::NONE);
    hit_box->set_layer_dst(CollisionLayer::PLAYER);
    hit_box->set_size({195, 10});

    this->position = position;
    this->velocity = {move_left ? -SPEED_MOVE : SPEED_MOVE, 0};
}

Sword::~Sword()
{
    CollisionManager::initstate().destroy_collision_box(hit_box);
}

void Sword::on_update(float delta)
{
    position += velocity * delta;
    animation.set_position(position);
    hit_box->set_position(position);

    animation.on_update(delta);

    if (position.x <= -200 || position.x >= Game::initstate().get_windowwidth())
    {
        is_valid = false;
    }
}
void Sword::on_render(const Camera &camera)
{
    animation.on_render(camera);
}

bool Sword::check_valid() const
{
    return is_valid;
}