#include "bullet_time_manager.h"

#include "SDL.h"

BulletTimeManager &BulletTimeManager::instance()
{
    static BulletTimeManager manager;
    return manager;
}

void BulletTimeManager::post_process(const Camera &camera) const
{
    uint8_t alpha = (uint8_t)(150 * progress);
    camera.post_process(alpha);
}

void BulletTimeManager::set_status(Status status)
{
    this->status = status;
}

float BulletTimeManager::on_update(float delta)
{
    float delta_progress = SPEED_PROGRESS * delta;
    progress += delta_progress * (status == Status::ENTERING ? 1 : -1);

    if (progress < 0)
        progress = 0;
    if (progress > 1.0f)
        progress = 1.0f;

    return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

float BulletTimeManager::lerp(float start, float end, float t)
{
    return (1 - t) * start + t * end;
}