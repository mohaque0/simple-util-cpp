#include "Thread.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include "system/thread/Lock.hpp"

#define THREAD_SUCCESS 0
#define THREAD_FAILURE 1

namespace {

class Falsifier {
	bool &value;
public:
	Falsifier(bool &b) : value(b) {}
	~Falsifier() { value = false; }
};

static void *runFunction(void *arg) {
	System::Thread::ThreadInfo &info = *(System::Thread::ThreadInfo *)arg;
	Falsifier r(info.running);
	info.f();
	info.running = false;
	return NULL;
}

}

namespace System {
namespace Thread {

Thread::Thread() :
	mutex(),
	tid(0),
	info(std::make_unique<ThreadInfo>([]() -> void{}))
{}

Thread::Thread(Thread &&other) :
	mutex(std::move(other.mutex)),
	tid(other.tid),
	info(std::move(other.info))
{
	// Prevents destructor of other variable from doing bad stuff.
	other.tid = 0;
}

Thread::Thread(std::function<void(void)> f_) :
	mutex(),
	tid(0),
	info(std::make_unique<ThreadInfo>(f_))
{}

Thread::~Thread()
{
	// Check exists only in case this thread was moved.
	if (tid == 0) {
		return;
	}

	stop();
}

int Thread::setFunction(std::function<void ()> f)
{
	Lock lock(mutex);
	if (info->running) {
		return THREAD_FAILURE;
	}

	info->f = f;
	return THREAD_SUCCESS;
}

int Thread::run()
{
	Lock lock(mutex);
	if (info->running) {
		return THREAD_FAILURE;
	}

	// Set it before. So if the thread exits fast, the value is correct.
	info->running = true;
	int ret = pthread_create(&tid, NULL, runFunction, info.get());
	if (ret) {
		// Error.
		info->running = false;
		return ret;
	}

	return THREAD_SUCCESS;
}

int Thread::stop()
{
	Lock lock(mutex);
	if (!info->running) {
		return THREAD_FAILURE;
	}

	int ret = pthread_cancel(tid);
	if (ret) {
		// Error.
		return ret;
	}

	info->running = false;

	// Prevent the destructor from doing bad things.
	tid = 0;

	return THREAD_SUCCESS;
}

int Thread::join()
{
	Lock lock(mutex);
	if (!info->running) {
		return THREAD_FAILURE;
	}

	void *retVal;
	pthread_join(tid, &retVal);

	return THREAD_SUCCESS;
}

bool Thread::isRunning() const
{
	Lock lock(mutex);
	return info->running;
}

}
}

#elif defined(PLATFORM_WINDOWS)

#endif
