#include "Timer.h"

uint64_t Timer::get_time(uint64_t timeStep) {
    // get current UTC time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    uint64_t currentTime64 = static_cast<uint64_t>(currentTime);

    // calculate counter based on current time and time step
    uint64_t counter = currentTime64 / timeStep;
    return counter;
}

uint64_t Timer::get_countdown() {
    // get current UTC time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    uint64_t currentTime64 = static_cast<uint64_t>(currentTime);

    // time step
    uint64_t timeStep = 30;

    // calculate counter based on current time and time step
    uint64_t pre_cnt = (currentTime64 % timeStep);
    uint64_t counter = 30 - pre_cnt;
    //std::cout << "Precount: " << pre_cnt << ", Counter: " << counter << std::endl;
    return counter;
}
