#include "ConditionVariable.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

namespace System {
namespace Thread {

// TODO: What to do about pthread functions that return failure?

ConditionVariable::ConditionVariable()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

ConditionVariable::~ConditionVariable()
{
	notifyAll();
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void ConditionVariable::wait()
{
	pthread_cond_wait(&cond, &mutex);
}

void ConditionVariable::notify()
{
	pthread_cond_signal(&cond);
}

void ConditionVariable::notifyAll()
{
	pthread_cond_broadcast(&cond);
}

}
}

#elif defined(PLATFORM_WINDOWS)

#endif

