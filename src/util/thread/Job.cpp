#include "Job.hpp"

#include "system/thread/Lock.hpp"
#include "util/thread/Task.hpp"

namespace Util {
namespace Thread {

void Job::addTask(std::unique_ptr<Task> task)
{
	System::Thread::Lock lock(internal->mutex);
	++internal->expectedTasks;
	internal->tasks.push_back(std::move(task));
}

Job::Job() :
	internal(std::make_shared<JobInternal>(0))
{}

Job::Job(Job &&other) :
	internal(other.internal)
{}

Job::~Job()
{}

void Job::waitForCompletion()
{
	internal->completionNotifier.wait();
	internal->completionNotifier.post();
}

auto Job::begin() -> decltype(internal->tasks.begin())
{
	return internal->tasks.begin();
}

auto Job::end() -> decltype(internal->tasks.end())
{
	return internal->tasks.end();
}

size_t Job::numCompleted() const
{
	return internal->tasks.size();
}

size_t Job::size() const
{
	return internal->expectedTasks;
}

void Job::JobInternal::notifyCompletion(size_t index, std::unique_ptr<Task> task)
{
	System::Thread::Lock lock(mutex);

	// Guarantee order is preserved.
	if (index == tasks.size() - 1) {
		tasks.push_back(std::move(task));
	} else {
		auto it = tasks.begin();
		for (int i = 0; i < index; i++) {
			++it;
		}
		tasks.insert(it, std::move(task));
	}

	if (tasks.size() == expectedTasks) {
		completionNotifier.post();
	}
}

}
}
