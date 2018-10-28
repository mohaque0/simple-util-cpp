#include "Lock.hpp"

#include "system/thread/Mutex.hpp"

namespace System {
namespace Thread {

Lock::Lock(const Mutex &mutex_) :
	mutex(mutex_)
{
	mutex.lock();
}

Lock::~Lock()
{
	mutex.unlock();
}

}
}
