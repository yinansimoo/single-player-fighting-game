#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void restart();
    void pause();
    void resume();
    void set_wait_time(float val);
    void set_on_shot(bool flag);
    void set_on_timeout(std::function<void()> on_timeout);
    void update(float delta);

private:
    float pass_time = 0;
    float wait_time = 0;
    bool shotted = false;
    bool paused = false;
    bool on_shot = false;
    std::function<void()> on_timeout;
};

#endif