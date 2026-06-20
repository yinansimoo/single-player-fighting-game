#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "barb.h"
#include "sword.h"
#include "character.h"

class Enemy : public Character
{
public:
    Enemy();
    ~Enemy();

    void on_update(float delta) override;
    void on_render(const Camera &camera) const override;

    void on_hurt() override;

    void set_facing_left(bool flag);
    bool get_facing_left() const;
    void set_dashing_in_air(bool flag);
    bool get_dashing_in_air() const;
    void set_dashing_on_floor(bool flag);
    bool get_dashing_on_floor() const;
    void set_throwing_silk(bool flag);
    bool get_throwing_silk() const;

    void throw_barbs();
    void throw_sword();

    void on_dash();
    void on_throw_silk();

    void init_animationGroup(const std::string &id, float interval, bool loop, const std::string &resource_id);
    void init_animation(Animation *animation, float interval, bool loop, const std::string &resource_id);

private:
    bool is_throwing_silk = false;
    bool is_dashing_in_air = false;
    bool is_dashing_on_floor = false;

    Animation animation_silk;
    AnimationGroup animation_dash_in_air_vfx;
    AnimationGroup animation_dash_on_floor_vfx;
    Animation *current_dash_animation = nullptr;

    std::vector<Sword *> sword_list;
    std::vector<Barb *> barb_list;
    CollisionBox *box_silk = nullptr;
};

#endif