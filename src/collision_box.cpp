#include "collision_box.h"

void CollisionBox::set_enable(bool flag)
{
    this->enabled = flag;
}

void CollisionBox::set_size(const Vector2 &size)
{
    this->size = size;
}

void CollisionBox::set_position(const Vector2 &position)
{
    this->position = position;
}

void CollisionBox::set_layer_src(CollisionLayer layer)
{
    this->layer_src = layer;
}

void CollisionBox::set_layer_dst(CollisionLayer layer)
{
    this->layer_dst = layer;
}

void CollisionBox::set_on_collide(std::function<void()> on_collide)
{
    this->on_collide = on_collide;
}

const Vector2 &CollisionBox::get_size() const
{
    return size;
}