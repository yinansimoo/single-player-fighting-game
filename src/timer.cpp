#include "timer.h"

void Timer::restart()
{
    this->pass_time = 0;
    this->shotted = false;
}

void Timer::pause()
{
    this->paused = true;
}

void Timer::resume()
{
    this->paused = false;
}

void Timer::set_wait_time(float val)
{
    this->wait_time = val;
}

void Timer::set_on_shot(bool flag)
{
    this->on_shot = flag;
}

void Timer::set_on_timeout(std::function<void()> on_timeout)
{
    this->on_timeout = on_timeout;
}

void Timer::update(float delta)
{
    if (paused)
    {
        return;
    }

    pass_time += delta;
    if (pass_time >= wait_time)
    {
        bool can_shot = (!on_shot) || (on_shot && !shotted);
        shotted = true;
        if (can_shot && on_timeout)
        {
            on_timeout();
        }
        pass_time -= wait_time;
    }
}