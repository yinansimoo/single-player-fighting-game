#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "collision_box.h"
#include "camera.h"

#include <vector>

class CollisionManager
{
public:
    static CollisionManager &initstate();

    CollisionBox *create_collision_box();
    void destroy_collision_box(CollisionBox *collisionbox);

    bool is_collision(CollisionBox *box1, CollisionBox *box2);
    void process_collide();
    void on_debug_render(const Camera &camera);

private:
    std::vector<CollisionBox *> collision_box_list;

private:
    CollisionManager() = default;
    ~CollisionManager() = default;
    CollisionManager(const CollisionManager &) = delete;
    CollisionManager &operator=(const CollisionManager &) = delete;
};

#endif