#ifndef _SWORD_H_
#define _SWORD_H_

#include "animation.h"
#include "collision_box.h"

class Sword
{
public:
    Sword(const Vector2 &position, bool move_left);
    ~Sword();

    void on_update(float delta);
    void on_render(const Camera &camera);

    bool check_valid() const;

private:
    const float SPEED_MOVE = 1250.0f;

private:
    Vector2 position;
    Vector2 velocity;
    Animation animation;
    bool is_valid = true;            //  是否可见
    CollisionBox *hit_box = nullptr; //  攻击碰撞箱
};

#endif