#ifndef _BARB_H_
#define _BARB_H_

#include "animation.h"
#include "collision_box.h"

class Barb
{
public:
    Barb();
    ~Barb();

    void on_update(float delta);
    void on_render(const Camera &camera) const;

    void set_position(const Vector2 &position);

    bool check_valid() const;

private:
    enum class State
    {
        IDLE,
        AIM,
        DASH,
        BREAK
    };

    const float SPEED_DASH = 1500.0f;

private:
    Timer timer_idle;
    Timer timer_aim;
    int diff_period = 0;
    bool is_valid = true;
    float total_delta_time = 0;

    Vector2 velocity;
    Vector2 base_position;
    Vector2 current_position;

    CollisionBox *box = nullptr;
    State state = State::IDLE;

    Animation animation_loose;
    Animation Animation_break;
    Animation *current_animation = nullptr;

private:
    void on_break();
};

#endif