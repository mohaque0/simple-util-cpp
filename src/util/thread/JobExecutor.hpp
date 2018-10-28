#ifndef JOBEXECUTOR_HPP
#define JOBEXECUTOR_HPP

#include "system/thread/Thread.hpp"
#include "util/thread/BlockingQueue.hpp"
#include "util/thread/Job.hpp"
#include "util/thread/Task.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace Util {
namespace Thread {

class JobTask {
public:
	std::shared_ptr<Job::JobInternal> jobData;
	size_t index;
	std::unique_ptr<Task> original;

public:
	JobTask(std::shared_ptr<Job::JobInternal> &jobData_, size_t index_, std::unique_ptr<Task> original_);
	JobTask(JobTask &&other);
	~JobTask();
	void run();
};

/**
 * @brief The JobExecutor class
 *
 * This executes submitted tasks. The tasks are owned by the Job class.
 */
class JobExecutor
{
	std::vector<System::Thread::Thread> threads;
	BlockingQueue<JobTask> taskQueue;

	/**
	 * @brief submit_internal
	 *
	 * This takes all the tasks in the job and adds them to the queue.
	 *
	 * @param job
	 * @return
	 */
	Job submit_internal(Job &job);

	/**
	 * @brief submit_internal
	 * @param job
	 * @param t
	 * @return
	 */
	Job submit_internal(Job &job, std::unique_ptr<Task> &&t);

	Job submit_internal(Job &job, std::function<void(void)> f);

	template<class T, class... Args>
	Job submit_internal(Job &job, std::unique_ptr<T>&& t, Args&&... args) {
		job.addTask(std::move(t));
		return submit_internal(job, std::forward<Args>(args)...);
	}

public:
	JobExecutor(unsigned int threads);
	~JobExecutor();

	const size_t numThreads() const;

	void submit(Job &job);

	template<class... Args>
	Job submit(Args&&... args) {
		Job job;
		return submit_internal(job, std::forward<Args>(args)...);
	}

	void shutdown();
};

}
}

#endif // JOBEXECUTOR_HPP
