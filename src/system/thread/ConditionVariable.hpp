#ifndef CONDITIONVARIABLE_HPP
#define CONDITIONVARIABLE_HPP

#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include <pthread.h>

namespace System {
namespace Thread {

class ConditionVariable
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
public:
    ConditionVariable();
	~ConditionVariable();

	void wait();

	/**
	 * @brief notify
	 *
	 * Wakes up at least a single thread waiting on this variable.
	 */
	void notify();

	/**
	 * @brief notifyAll
	 *
	 * Wakes up all threads waiting on this variable.
	 */
	void notifyAll();
};

}
}

#elif defined(PLATFORM_WINDOWS)

namespace System {
namespace Thread {

class ConditionVariable
{
public:
    ConditionVariable();
	~ConditionVariable();

	void wait();

	/**
	 * @brief notify
	 *
	 * Wakes up at least a single thread waiting on this variable.
	 */
	void notify();

	/**
	 * @brief notifyAll
	 *
	 * Wakes up all threads waiting on this variable.
	 */
	void notifyAll();
};

}
}

#endif

#endif // CONDITIONVARIABLE_HPP
