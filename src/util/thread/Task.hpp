#ifndef TASK_HPP
#define TASK_HPP

namespace Util {
namespace Thread {

class Task {
public:
	Task();
	virtual ~Task();
	virtual void run() = 0;
};

}
}

#endif // TASK_HPP
