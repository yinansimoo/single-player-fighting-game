#include "collision_manager.h"

CollisionManager &CollisionManager::initstate()
{
    static CollisionManager manager;
    return manager;
}

CollisionBox *CollisionManager::create_collision_box()
{
    CollisionBox *box = new CollisionBox();
    collision_box_list.push_back(box);
    return box;
}

void CollisionManager::destroy_collision_box(CollisionBox *collisionbox)
{
    collision_box_list.erase(std::remove(collision_box_list.begin(), collision_box_list.end(), collisionbox),
                             collision_box_list.end());
    delete collisionbox;
}

bool CollisionManager::is_collision(CollisionBox *box1, CollisionBox *box2)
{
    bool x_collision = (std::max(box1->position.x + box1->size.x / 2, box2->position.x + box2->size.x / 2) -
                        std::min(box1->position.x - box1->size.x / 2, box2->position.x - box2->size.x / 2)) <=
                       (box1->size.x + box2->size.x);
    bool y_collision = (std::max(box1->position.y + box1->size.y / 2, box2->position.y + box2->size.y / 2) -
                        std::min(box1->position.y - box1->size.y / 2, box2->position.y - box2->size.y / 2)) <=
                       (box1->size.y + box2->size.y);
    return x_collision && y_collision;
}

void CollisionManager::process_collide()
{
    for (auto box_src : collision_box_list)
    {
        if (!box_src->enabled || box_src->layer_dst == CollisionLayer::NONE)
        {
            continue;
        }

        for (auto box_dst : collision_box_list)
        {
            if (!box_dst->enabled || box_src == box_dst || box_src->layer_dst != box_dst->layer_src)
            {
                continue;
            }

            if (is_collision(box_src, box_dst) && box_dst->on_collide)
            {
                box_dst->on_collide();
            }
        }
    }
}

void CollisionManager::on_debug_render(const Camera &camera)
{
    for (auto box : collision_box_list)
    {
        SDL_Color color;
        if (box->enabled)
        {
            color = {255, 180, 180, 255};
        }
        else
        {
            color = {120, 120, 255, 255};
        }

        SDL_Rect dst = {(int)(box->position.x - box->size.x / 2),
                        (int)(box->position.y - box->size.y / 2),
                        (int)(box->size.x),
                        (int)(box->size.y)};

        camera.on_debug(&dst, color);
    }
}
