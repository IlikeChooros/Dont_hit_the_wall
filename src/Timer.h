#ifndef _TIMER_H
#define _TIMER_H

#include <Arduino.h>
void timer_do_nothing(void);
class Timer
{
    private:
        uint16_t _interval;
        void (*_on_times_up)(void);
        uint32_t _start_time;
    public:
        Timer(uint16_t interval);
        void start();
        void check();
        void on_times_up(void(*)(void));
        void set_interval(uint16_t interval);

};


#endif