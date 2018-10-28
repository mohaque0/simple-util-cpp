#ifndef CONSOLESINK_HPP
#define CONSOLESINK_HPP

#include "util/logging/LogLevel.hpp"
#include "util/logging/LogSink.hpp"
#include "util/String.hpp"

#include <functional>

namespace Util {
namespace Logging {

class ConsoleSink : public LogSink
{
	std::function<String(String)> formatter;
	LogLevel level;
public:
	ConsoleSink();
	ConsoleSink(LogLevel level_);
	ConsoleSink(std::function<const String(const String &)> formatter_, LogLevel level_);
	~ConsoleSink();

	// LogSink interface
	void log(LogLine &line);
};

}
}


#endif // CONSOLESINK_HPP
