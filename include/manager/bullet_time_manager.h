#ifndef _BULLET_TIME_MANAGER_H_
#define _BULLET_TIME_MANAGER_H_

#include "camera.h"

class BulletTimeManager
{
public:
    enum class Status
    {
        ENTERING,
        EXITING
    };

public:
    static BulletTimeManager &instance();

    void post_process(const Camera &camera) const;
    void set_status(Status status);

    float on_update(float delta);

private:
    float progress = 0;
    Status status = Status::EXITING;
    const float SPEED_PROGRESS = 2.0f;
    const float DST_DELTA_FACTOR = 0.35f;
    const float DST_COLOR_FACTOR = 0.35f;

private:
    BulletTimeManager() = default;
    ~BulletTimeManager() = default;
    BulletTimeManager(const BulletTimeManager &) = delete;
    BulletTimeManager &operator=(const BulletTimeManager &) = delete;

    float lerp(float start, float end, float t);
};

#endif