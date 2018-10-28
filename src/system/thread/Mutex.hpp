#ifndef MUTEX_HPP
#define MUTEX_HPP

#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include <memory>
#include <pthread.h>

namespace System {
namespace Thread {

class Mutex
{
	mutable std::unique_ptr<pthread_mutex_t[]> mutex;

public:
    Mutex();
	Mutex(Mutex &&other);
	Mutex(const Mutex &other) = delete;
	~Mutex();
	int lock() const;
	int unlock() const;
};

}
}

#elif defined(PLATFORM_WINDOWS)

namespace System {
namespace Thread {

class Mutex
{

public:
    Mutex();
	Mutex(Mutex &&other);
	Mutex(const Mutex &other) = delete;
	~Mutex();
	int lock() const;
	int unlock() const;
};

}
}

#endif

#endif // MUTEX_HPP
