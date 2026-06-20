#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "Vector2.h"
#include "collision_layer.h"

#include <functional>

class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;

public:
    void set_enable(bool flag);
    void set_size(const Vector2 &size);
    void set_position(const Vector2 &position);
    void set_layer_src(CollisionLayer layer);
    void set_layer_dst(CollisionLayer layer);
    void set_on_collide(std::function<void()> on_collide);

    const Vector2 &get_size() const;

private:
    Vector2 position;
    Vector2 size;
    bool enabled = true;
    std::function<void()> on_collide;
    CollisionLayer layer_src = CollisionLayer::NONE;
    CollisionLayer layer_dst = CollisionLayer::NONE;

private:
    CollisionBox() = default;
    ~CollisionBox() = default;
};

#endif