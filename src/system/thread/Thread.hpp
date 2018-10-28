#ifndef THREAD_HPP
#define THREAD_HPP

#include "system/thread/Mutex.hpp"
#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include <functional>
#include <memory>
#include <pthread.h>

namespace System {
namespace Thread {

namespace {
class ThreadInfo {
public:
	std::function<void(void)> f;
	bool running;

	ThreadInfo(std::function<void(void)> f_) :
		f(f_),
		running(false)
	{}
	ThreadInfo(ThreadInfo &&other) = delete;
	ThreadInfo(const ThreadInfo& other) = delete;
	~ThreadInfo() {}
};
}


class Thread
{
	Mutex mutex;
	pthread_t tid;
	std::unique_ptr<ThreadInfo> info;

public:
	Thread();
	Thread(Thread &&other);
	Thread(std::function<void(void)> f_);
	~Thread();

	/**
	 * @brief setFunction
	 *
	 * Sets the function this thread executes on running to the specified
	 * function.
	 *
	 * @param f
	 * @return
	 */
	int setFunction(std::function<void(void)> f);

	/**
	 * @brief run
	 *
	 * Runs the thread.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int run();

	/**
	 * @brief stop
	 *
	 * Stops the thread.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int stop();

	/**
	 * @brief join
	 *
	 * Suspends the caller until this thread completes execution.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int join();

	bool isRunning() const;
};

}
}

#elif defined(PLATFORM_WINDOWS)

#include <functional>
#include <memory>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

namespace System {
namespace Thread {

namespace {
class ThreadInfo {
public:
	std::function<void(void)> f;
	bool running;

	ThreadInfo(std::function<void(void)> f_) :
		f(f_),
		running(false)
	{}
	ThreadInfo(ThreadInfo &&other) = delete;
	ThreadInfo(const ThreadInfo& other) = delete;
	~ThreadInfo() {}
};
}

class Thread
{
	Mutex mutex;
	DWORD tid;
	std::unique_ptr<ThreadInfo> info;

public:
	Thread();
	Thread(Thread &&other);
	Thread(std::function<void(void)> f_);
	~Thread();

	/**
	 * @brief setFunction
	 *
	 * Sets the function this thread executes on running to the specified
	 * function.
	 *
	 * @param f
	 * @return
	 */
	int setFunction(std::function<void(void)> f);

	/**
	 * @brief run
	 *
	 * Runs the thread.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int run();

	/**
	 * @brief stop
	 *
	 * Stops the thread.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int stop();

	/**
	 * @brief join
	 *
	 * Suspends the caller until this thread completes execution.
	 *
	 * @return Returns 0 on success and non-zero on failure.
	 */
	int join();

	bool isRunning() const;
};

}
}

#endif

#endif // THREAD_HPP
