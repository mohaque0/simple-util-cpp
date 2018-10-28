#ifndef THREADSAFEQUEUE
#define THREADSAFEQUEUE

#include "system/thread/Lock.hpp"
#include "system/thread/Mutex.hpp"

#include <queue>

#include <iostream>
#include <thread>


namespace Util {
namespace Thread {

template<typename T>
class ThreadSafeQueue
{
	System::Thread::Mutex mutex;
	std::queue<T> queue;

public:
	ThreadSafeQueue() : mutex() {}
	~ThreadSafeQueue() {}

	bool isEmpty() const {
		System::Thread::Lock lock(mutex);
		return queue.empty();
	}

	size_t size() const {
		System::Thread::Lock lock(mutex);
		return queue.size();
	}

	void push(T&& value) {
		System::Thread::Lock lock(mutex);
		queue.push(std::move(value));
	}

	void push(const T& value) {
		System::Thread::Lock lock(mutex);
		queue.push(value);
	}

	template <class... Args>
	void emplace(Args&&... args) {
		System::Thread::Lock lock(mutex);
		queue.emplace(std::forward<Args>(args)...);
	}

	void pop() {
		System::Thread::Lock lock(mutex);
		queue.pop();
	}

	T &front() {
		System::Thread::Lock lock(mutex);
		return queue.front();
	}

	const T &front() const {
		System::Thread::Lock lock(mutex);
		return queue.front();
	}

	T &back() {
		System::Thread::Lock lock(mutex);
		return queue.back();
	}

	const T &back() const {
		System::Thread::Lock lock(mutex);
		return queue.back();
	}

	std::queue<T> &q() { return queue; }
};

}
}


#endif // THREADSAFEQUEUE

