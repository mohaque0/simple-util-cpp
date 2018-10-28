#ifndef LOCK_HPP
#define LOCK_HPP

#include "system/Platform.hpp"

namespace System {
namespace Thread {

class Mutex;

class Lock
{
	const Mutex &mutex;
public:
	Lock(const Mutex &mutex_);
	Lock(const Lock& other) = delete;
	~Lock();
};

}
}

#endif // LOCK_HPP
