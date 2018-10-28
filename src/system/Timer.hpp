/*
 * Timer.hpp
 *
 *  Created on: Jul 27, 2013
 *      Author: mhaque
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "system/Platform.hpp"

// Is there a better way to make this platform independent?
#ifdef PLATFORM_MAC

#include <mach/mach.h>
#include <mach/clock.h>

namespace System {

class Timer {
private:
    clock_serv_t clock;
    mach_timespec_t lastTime;

public:
    Timer();
    virtual ~Timer();
    const double elapsedSeconds() const;
    void reset();
};

}

#endif

#ifdef PLATFORM_LINUX

#include <time.h>

namespace System {

class Timer {
private:
	timespec lastTime;

public:
	Timer();
	virtual ~Timer();
	const double elapsedSeconds() const;
	void reset();
};

}

#endif

#ifdef PLATFORM_WINDOWS

// TODO: This is platform independent and could replace the older implementation.
#include <chrono>

namespace System {

class Timer {
	std::chrono::high_resolution_clock::time_point lastTime;
public:
	Timer();
	virtual ~Timer();
	const double elapsedSeconds() const;
	void reset();
};

}

#endif

#endif /* TIMER_HPP_ */
