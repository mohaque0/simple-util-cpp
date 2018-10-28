/*
 * Timer.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: mhaque
 */

#include "Timer.hpp"

#ifdef PLATFORM_MAC

namespace System {

Timer::Timer()
{
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
    clock_get_time(clock, &lastTime);
}

Timer::~Timer() {
    mach_port_deallocate(mach_task_self(), clock);
}

const double Timer::elapsedSeconds() const {
    mach_timespec_t current;
    clock_get_time(clock, &current);
    long nano = current.tv_nsec - lastTime.tv_nsec;
    unsigned long secs = current.tv_sec - lastTime.tv_sec;
    return secs + ((float)nano / (float)1000000000);
}

void Timer::reset() {
    clock_get_time(clock, &lastTime);
}

}

#endif

#ifdef PLATFORM_LINUX

#include <time.h>

namespace System {

Timer::Timer()
{
    clock_gettime(CLOCK_REALTIME, &lastTime);
}

Timer::~Timer() {}

const double Timer::elapsedSeconds() const {
	timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
	long nano = current.tv_nsec - lastTime.tv_nsec;
	unsigned long secs = current.tv_sec - lastTime.tv_sec;
	return secs + ((float)nano / (float)1000000000);
}

void Timer::reset() {
    clock_gettime(CLOCK_REALTIME, &lastTime);
}

}

#endif

#ifdef PLATFORM_WINDOWS

namespace System {

Timer::Timer() :
    lastTime(std::chrono::high_resolution_clock::now())
{}

Timer::~Timer() {}

const double Timer::elapsedSeconds() const {
	std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(current - lastTime);
	return ((float)ms.count() / (float)1000);
}

void Timer::reset() {
    lastTime = std::chrono::high_resolution_clock::now();
}

}


#endif
