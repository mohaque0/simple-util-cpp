#include "Logging.hpp"

#include "system/thread/Lock.hpp"
#include "system/thread/Thread.hpp"
#include <iostream>

#include "util/exception/Exception.hpp"

namespace Util {
namespace Logging {

LoggingSystem::LoggingSystem() :
	shouldRun(true)
{
	msgProcessor = std::make_unique<System::Thread::Thread>();
	msgProcessor->setFunction(
		[this] () -> void {
			while (this->shouldRun) {
				LogLine line = this->queue.pop();
				System::Thread::Lock lock(sinkMutex);
				for (std::unique_ptr<LogSink> &sink : sinks) {
					sink->log(line);
				}
			}
		}
	);
	msgProcessor->run();
}

LoggingSystem::~LoggingSystem()
{
	queue.push(LogLine(LogLevel::Trace(), "Ending logging."));
	while (queue.size() > 0) {
		// Wait for message processor to finish.
	}
	shouldRun = false;
	msgProcessor->join();
}

void LoggingSystem::addSink(std::unique_ptr<LogSink> sink)
{
	System::Thread::Lock lock(sinkMutex);
	sinks.push_back(std::move(sink));
}

LoggingSystem &log() {
	static LoggingSystem system;
	return system;
}

}
}
