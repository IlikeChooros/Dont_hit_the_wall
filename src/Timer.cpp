#include "Timer.h"

void timer_do_nothing()
{
    return;
}

Timer::Timer (uint16_t interval)
{
    _interval=interval;
    _start_time=0;
    this->on_times_up(timer_do_nothing);
}

void Timer::start()
{
    _start_time=millis();
}

void Timer::check()
{
    uint32_t currentTime = millis();

    if (currentTime-_start_time>_interval)
    {
        this->_on_times_up();
        _start_time=currentTime;
    }
}

void Timer::on_times_up(void(*on_times_up)(void))
{
    this->_on_times_up=on_times_up;
}

void Timer::set_interval(uint16_t interval)
{
    _interval=interval;
}
