#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "system/Platform.hpp"

#ifdef PLATFORM_MAC

#include <dispatch/dispatch.h>

namespace System {
namespace Thread {

class Semaphore
{
	dispatch_semaphore_t s;
	unsigned int v;
public:
	/**
	 * @brief Semaphore
	 * Create a semaphore with value 0.
	 */
	Semaphore();
	Semaphore(unsigned int value);
	Semaphore(const Semaphore &other) = delete;
	~Semaphore();
	void post();
	void wait();
	int value();
};

}
}

#endif
#ifdef PLATFORM_LINUX

#include <semaphore.h>

namespace System {
namespace Thread {

class Semaphore
{
	sem_t s;
public:
	/**
	 * @brief Semaphore
	 * Create a semaphore with value 0.
	 */
	Semaphore();
	Semaphore(unsigned int value);
	Semaphore(const Semaphore &other) = delete;
	~Semaphore();
	void post();
	void wait();
	int value();
};

}
}

#endif
#ifdef PLATFORM_WINDOWS

namespace System {
namespace Thread {

class Semaphore
{
public:
	/**
	 * @brief Semaphore
	 * Create a semaphore with value 0.
	 */
	Semaphore();
	Semaphore(unsigned int value);
	Semaphore(const Semaphore &other) = delete;
	~Semaphore();
	void post();
	void wait();
	int value();
};

}
}

#endif

#endif // SEMAPHORE_HPP
