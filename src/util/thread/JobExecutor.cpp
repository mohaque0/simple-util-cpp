#include "JobExecutor.hpp"

#include "system/thread/ConditionVariable.hpp"
#include "system/thread/Lock.hpp"
#include "system/thread/Mutex.hpp"
#include "system/thread/Thread.hpp"
#include "util/exception/Exception.hpp"
#include "util/thread/Job.hpp"

#include <iostream>

namespace Util {
namespace Thread {

namespace {

class UnitFunctionTask : public Task {
	std::function<void(void)> f;

public:
	UnitFunctionTask(std::function<void(void)> f_) : f(f_) {}
	~UnitFunctionTask() {}

	// Task interface
	void run() {
		f();
	}
};

}

JobTask::JobTask(std::shared_ptr<Job::JobInternal> &jobData_, size_t index_, std::unique_ptr<Task> original_)  :
	jobData(jobData_),
	index(index_),
	original(std::move(original_))
{}

JobTask::JobTask(JobTask &&other) :
	jobData(other.jobData),
	index(other.index),
	original(std::move(other.original))
{}

JobTask::~JobTask()
{}

void JobTask::run()
{
	original->run();
	jobData->notifyCompletion(index, std::move(original));
}

Job JobExecutor::submit_internal(Job &job)
{
	submit(job);
	return std::move(job);
}

Job JobExecutor::submit_internal(Job &job, std::unique_ptr<Task> &&t)
{
	job.addTask(std::move(t));
	return submit_internal(job);
}

Job JobExecutor::submit_internal(Job &job, std::function<void ()> f)
{
	job.addTask(std::make_unique<UnitFunctionTask>(f));
	return submit_internal(job);
}

JobExecutor::JobExecutor(unsigned int numThreads) :
	threads()
{
	BlockingQueue<JobTask> &taskQueue =
		this->taskQueue;

	for (int i = 0; i < numThreads; i++) {
		threads.emplace_back();
		threads.back().setFunction([&taskQueue]() -> void {
			while (true) {
				JobTask t(std::move(taskQueue.pop()));
				try {
					t.run();
				} catch (Util::Exception::Exception &e) {
					std::cout << e.what() << std::endl;
				}
			}
		});
		threads.back().run();
	}
}

JobExecutor::~JobExecutor()
{
	for (auto &thread : threads) {
		thread.stop();
	}
}

const size_t JobExecutor::numThreads() const
{
	return threads.size();
}

void JobExecutor::submit(Job &job)
{
	// We don't want the executor threads to put their tasks back
	// on until we're done.
	System::Thread::Lock lock(job.internal->mutex);
	for (int i = job.size(); i > 0; --i) {
		// Move all of the job's tasks onto the queue.
		taskQueue.emplace(
			job.internal,
			i-1,
			std::move(job.internal->tasks.back())
		);
		job.internal->tasks.pop_back();
	}
}

}
}

