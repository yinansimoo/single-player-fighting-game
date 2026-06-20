#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Vector2.h"
#include "animation.h"
#include "camera.h"
#include "collision_box.h"
#include "state_machine.h"

#include <string>
#include <unordered_map>

class Character
{
public:
    Character();
    virtual ~Character();

protected:
    struct AnimationGroup
    {
        Animation left;
        Animation right;
    };

public:
    void decrease_hp();

    int get_hp() const;
    void set_position(const Vector2 &position);
    const Vector2 &get_position() const;
    void set_velocity(const Vector2 &velocity);
    const Vector2 &get_velocity() const;
    void set_gravity_enable(bool flag);
    CollisionBox *get_hit_box();
    CollisionBox *get_hurt_box();
    bool is_on_floor() const;
    float get_floor_y() const;
    void make_invulnerable();
    float get_logic_height();

    virtual void on_input(const SDL_Event &event);
    virtual void on_update(float delta);
    virtual void on_render(const Camera &camera) const;

    virtual void on_hurt();

    void switch_state(const std::string &id);
    void set_animation(const std::string &id);

protected:
    const float FLOOR_Y = 620;     //  地板竖直坐标
    const float GRAVITY = 980 * 2; //  重力

protected:
    int hp = 10; //  生命
    Vector2 position;
    Vector2 velocity;
    float logic_height = 0;                                         //  角色逻辑高度
    bool is_facing_left = true;                                     //  角色是否朝向左
    StateMachine state_machine;                                     //  逻辑状态机
    bool enable_gravity = true;                                     //  是否启用重力
    bool is_invulnerable = false;                                   // 是否无敌
    Timer timer_invulnerable_blink;                                 //  无敌闪烁定时器
    Timer timer_invulnerable_state;                                 //  无敌状态定时器
    bool is_blink_invisible = false;                                // 无敌闪烁
    CollisionBox *hit_box = nullptr;                                // 攻击碰撞箱
    CollisionBox *hurt_box = nullptr;                               //  受机碰撞箱
    AnimationGroup *current_animation = nullptr;                    //  当前角色动画
    std::unordered_map<std::string, AnimationGroup> animation_pool; // 角色动画池
};

#endif