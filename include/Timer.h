#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

class Timer
{
    public:
        static uint64_t get_time(uint64_t timestep=30);
        static uint64_t get_countdown();

    protected:

    private:
};

#endif // TIMER_H
