#ifndef JOB_HPP
#define JOB_HPP

#include "system/thread/Mutex.hpp"
#include "system/thread/Semaphore.hpp"

#include <memory>
#include <list>

namespace Util {
namespace Thread {

class JobExecutor;
class JobTask;
class Task;

class Job {
	friend class Util::Thread::JobExecutor;
	friend class Util::Thread::JobTask;

	// Keeping a unique_ptr of this class guarantees the job data
	// doesn't move in memory so that JobTask can reference it later.
	class JobInternal {
	public:
		System::Thread::Mutex mutex;
		System::Thread::Semaphore completionNotifier;
		size_t expectedTasks;
		std::list<std::unique_ptr<Task>> tasks;

		JobInternal(size_t expectedTasks_) :
			expectedTasks(expectedTasks_)
		{}
		JobInternal(JobInternal&& other) = delete;
		~JobInternal() {}

		void notifyCompletion(size_t index, std::unique_ptr<Task> task);
	};

	std::shared_ptr<JobInternal> internal;

public:
	Job();
	Job(Job&& other);
	~Job();

	/**
	 * @brief addTask
	 *
	 * Add a task to this job. These tasks are considered completed until
	 * they are submitted to an executor.
	 *
	 * @param task
	 */
	void addTask(std::unique_ptr<Task> task);
	void waitForCompletion();

	/**
	 * @brief begin
	 *
	 * Iterator pointing to the beginning of completed tasks.
	 *
	 * Note behavior is undefined if iterating over completed tasks before tasks are done.
	 */
	decltype(internal->tasks.begin()) begin();

	/**
	 * @brief begin
	 *
	 * Iterator pointing to the end of completed tasks.
	 *
	 * Note behavior is undefined if iterating over completed tasks before tasks are done.
	 */
	decltype(internal->tasks.end()) end();

	/**
	 * @brief numCompleted
	 * @return Number of this job's tasks which have completed. If the job
	 *         has not been submitted for execution, its tasks are considered
	 *         completed.
	 */
	size_t numCompleted() const;

	/**
	 * @brief size
	 * @return Total number of tasks in this job.
	 */
	size_t size() const;
};

}
}

#endif // JOB_HPP
