#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

class Player : public Character
{
public:
    enum class AttackDir
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

public:
    Player();
    ~Player() = default;

    void on_input(const SDL_Event &event) override;
    void on_update(float delta) override;
    void on_render(const Camera &camera) const override;
    void on_hurt() override;

    void set_rolling(bool flag);
    bool get_rolling() const;
    bool can_roll() const;

    void set_attacking(bool flag);
    bool get_attacking() const;
    bool can_attack() const;

    bool can_jump() const;

    int get_move_axis() const;

    AttackDir get_attack_dir() const;

    void on_jump();
    void on_land();
    void on_roll();
    void on_attack();

    void init_texture(const std::string &id, float interval, bool loop, const std::string &resource_id, int num);
    void init_animation(Animation *animation, float interval, bool loop, const std::string &resource_id, int num);

private:
    const float CD_ROLL = 0.75f;
    const float CD_ATTACK = 0.5f;
    const float SPEED_RUN = 300.0f;
    const float SPEED_JUMP = 780.0f;
    const float SPEED_ROLL = 800.0f;

private:
    Timer timer_roll_cd;
    bool is_rolling = false;
    bool is_roll_cd_comp = true;

    Timer timer_attack_cd;
    bool is_attacking = false;
    bool is_attack_cd_comp = true;

    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_jump_key_down = false;
    bool is_roll_key_down = false;
    bool is_attack_key_down = false;

    Animation animation_alash_up;
    Animation animation_alash_down;
    Animation animation_alash_left;
    Animation animation_alash_right;
    AttackDir attack_dir = AttackDir::RIGHT;
    Animation *current_slash_animation = nullptr;

    bool is_jump_vfx_visible = false;
    Animation animation_jump_vfx;

    bool is_land_vfx_visible = false;
    Animation animation_land_vfx;

private:
    void update_attack_dir(int x, int y);
};

#endif