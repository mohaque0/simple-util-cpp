#include "Semaphore.hpp"

#ifdef PLATFORM_MAC

namespace System {
namespace Thread {

Semaphore::Semaphore()
{
	// What to do on failure?
	s = dispatch_semaphore_create(0);
}

Semaphore::Semaphore(unsigned int value_) :
	v(value_)
{
	// What to do on failure?
	s = dispatch_semaphore_create(v);
}

Semaphore::~Semaphore()
{
	// What to do on failure?
	dispatch_release(s);
}

void Semaphore::post()
{
	++v;
	// What to do on failure?
	dispatch_semaphore_signal(s);
}

void Semaphore::wait()
{
	// What to do on failure?
	dispatch_semaphore_wait(s, DISPATCH_TIME_FOREVER);
	--v;
}

int Semaphore::value()
{
	return v;
}

}
}

#endif
#ifdef PLATFORM_LINUX

namespace System {
namespace Thread {

Semaphore::Semaphore()
{
	// What to do on failure?
	sem_init(&s, 0, 0);
}

Semaphore::Semaphore(unsigned int value)
{
	// What to do on failure?
	sem_init(&s, 0, value);
}

Semaphore::~Semaphore()
{
	// What to do on failure?
	sem_destroy(&s);
}

void Semaphore::post()
{
	// What to do on failure?
	sem_post(&s);
}

void Semaphore::wait()
{
	// What to do on failure?
	sem_wait(&s);
}

int Semaphore::value()
{
	int retVal;

	// What to do on failure?
	sem_getvalue(&s, &retVal);

	return retVal;
}

}
}

#endif
#ifdef PLATFORM_WINDOWS

#endif