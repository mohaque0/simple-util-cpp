#include "Mutex.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

namespace System {
namespace Thread {

Mutex::Mutex() :
	mutex(new pthread_mutex_t[1])
{
	pthread_mutex_init(&(mutex.get()[0]), NULL);
}

Mutex::Mutex(Mutex &&other) :
	mutex(move(other.mutex))
{}

Mutex::~Mutex()
{
	// Can be null if mutex was moved.
	if (mutex.get() != NULL) {
		pthread_mutex_destroy(&(mutex.get()[0]));
	}
}

int Mutex::lock() const
{
	return pthread_mutex_lock(&(mutex.get()[0]));
}

int Mutex::unlock() const
{
	return pthread_mutex_unlock(&(mutex.get()[0]));
}

}
}

#elif defined(PLATFORM_WINDOWS)



#endif