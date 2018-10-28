#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "system/thread/Mutex.hpp"

#include "util/String.hpp"
#include "util/logging/LogLevel.hpp"
#include "util/logging/LogLine.hpp"
#include "util/logging/LogSink.hpp"
#include "util/thread/BlockingQueue.hpp"

#include <cstdlib>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

namespace System {
namespace Thread {
class Thread;
}
}

namespace Util {
namespace Logging {

// Funtion prototype.
template <typename T>
String logString(T &t);

namespace {

/**
 * @brief parseLogArgs2
 *
 * This will run through the format string and replace all pairs of
 * non-escaped brackets with the argument whose index is specified
 * between the brackets. If no index is specified, then the argument
 * corresponding to index 'idx' will be used and idx will be incremented.
 * If the index is out of bounds then the bracket pair and the contents
 * within will not be replaced with anything.
 *
 * Characters are escaped with a preceding backslash ('\').
 *
 * @param format
 * @param args
 * @return
 */
template<class... Args>
String parseLogArgs(const char * const format, Args&&... args)
{
	String argStrings[] = {
		logString(args)...,
		String("")
	};
	constexpr auto numArgs = sizeof...(Args);

	if (numArgs == 0) {
		return String(format);
	}

	String retVal;
	bool escape = false;
	bool processArg = false;

	size_t runningIndex = 0;
	for (size_t i = 0; format[i] != '\0'; i++) {
		const char c = format[i];

		// We always print escaped characters.
		if (escape == true) {
			retVal += format[i];
			escape = false;

		// If it's not escaped process it.
		} else {

			// Escape the next character.
			if (c == '\\') {
				escape = true;
				continue;
			}

			// We found a parameter.
			if (processArg == false && c == '{') {
				processArg = true;

				// Get the argument index.
				int argIndex = atoi(&format[i+1]);
				if (format[i+1] == '}' || argIndex < 0 || argIndex >= numArgs) {
					argIndex = runningIndex++;
				}

				// Make sure it's in range. (Note the duplicate check in case
				// runningIndex is out of bounds.)
				if (argIndex < 0 || argIndex >= numArgs) {

					// Can't process this, ignore.
					retVal += '{';
					processArg = false;

				} else {
					retVal += argStrings[argIndex];
				}

				continue;
			}

			// We're not processing parameters after this character.
			if (processArg == true && c == '}') {
				processArg = false;
				continue;
			}

			// Don't print out characters if we're processing arguments (i.e. it's inside brackets).
			if (processArg == false) {
				retVal += format[i];
			}
		}
	}

	return retVal;
}

} // anonymous namespace

class LoggingSystem {
	Thread::BlockingQueue<LogLine> queue;
	bool shouldRun;

	System::Thread::Mutex sinkMutex;
	std::vector<std::unique_ptr<LogSink>> sinks;

	std::unique_ptr<System::Thread::Thread> msgProcessor;

public:
	LoggingSystem();
	~LoggingSystem();

	void shutdown();

	void addSink(std::unique_ptr<LogSink> sink);

	template <typename... ArgTypes>
	void error(const char *format, ArgTypes&&... args) {
		queue.emplace(LogLevel::Error(), std::move(parseLogArgs(format, std::forward<ArgTypes>(args)...)));
	}

	template <typename... ArgTypes>
	void warn(const char *format, ArgTypes&&... args) {
		queue.emplace(LogLevel::Warning(), std::move(parseLogArgs(format, std::forward<ArgTypes>(args)...)));
	}

	template <typename... ArgTypes>
	void info(const char *format, ArgTypes&&... args) {
		queue.emplace(LogLevel::Info(), std::move(parseLogArgs(format, std::forward<ArgTypes>(args)...)));
	}

	template <typename... ArgTypes>
	void debug(const char *format, ArgTypes&&... args) {
		queue.emplace(LogLevel::Debug(), std::move(parseLogArgs(format, std::forward<ArgTypes>(args)...)));
	}

	template <typename... ArgTypes>
	void trace(const char *format, ArgTypes&&... args) {
		queue.emplace(LogLevel::Trace(), std::move(parseLogArgs(format, std::forward<ArgTypes>(args)...)));
	}
};

/**
 * @brief logString Convert the given object into a String for logging.
 *
 * This is a template for converting an object of any type T into a String
 * based on how T overloads ostream& operator<<(ostream&, const T&).
 *
 * This template should be specialized if any special behavior is necessary.
 *
 * @param t Object to convert.
 * @return The String form of the object
 */
template <typename T>
String logString(const T &t) {
	std::stringstream str;
	str << t;
	return String(str.str().c_str());
}

template <typename T>
String logString(T &t) {
	return logString(const_cast<const T&>(t));
}

LoggingSystem& log();

}
}


#endif // LOGGING_HPP
