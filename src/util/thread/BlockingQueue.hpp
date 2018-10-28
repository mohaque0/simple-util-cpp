#ifndef BLOCKINGQUEUE_HPP
#define BLOCKINGQUEUE_HPP

#include "system/thread/Lock.hpp"
#include "system/thread/Mutex.hpp"
#include "system/thread/Semaphore.hpp"

#include <memory>
#include <queue>

namespace Util {
namespace Thread {

template<typename T>
class BlockingQueue
{
	System::Thread::Mutex mutex;
	System::Thread::Semaphore queuedItems;
	std::queue<T> queue;

public:
	BlockingQueue() {}
	~BlockingQueue() {}

	void push(T&& value) {
		System::Thread::Lock lock(mutex);
		queue.push(std::move(value));
		queuedItems.post();
	}

	template <class... Args>
	void emplace(Args&&... args) {
		System::Thread::Lock lock(mutex);
		queue.emplace(std::forward<Args>(args)...);
		queuedItems.post();
	}

	T pop() {
		queuedItems.wait();
		System::Thread::Lock lock(mutex);
		T retVal = std::move(queue.front());
		queue.pop();
		return retVal;
	}

	size_t size() const {
		return queue.size();
	}
};

}
}

#endif // BLOCKINGQUEUE_HPP
