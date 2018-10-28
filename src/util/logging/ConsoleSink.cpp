#include "ConsoleSink.hpp"

#include "util/logging/DefaultLogFormatter.hpp"
#include "util/logging/LogLine.hpp"

#include <iostream>

using namespace std;

namespace Util {
namespace Logging {

ConsoleSink::ConsoleSink() :
	formatter([] (const String &msg) -> const String { return DefaultLogFormatter()(msg); }),
	level(LogLevel::Info())
{}

ConsoleSink::ConsoleSink(LogLevel level_) :
	formatter([] (const String &msg) -> const String { return DefaultLogFormatter()(msg); }),
	level(level_)
{}

ConsoleSink::ConsoleSink(std::function<const String (const String&)> formatter_, LogLevel level_) :
	formatter(formatter_),
	level(level_)
{}

ConsoleSink::~ConsoleSink()
{}

void ConsoleSink::log(LogLine &line)
{
	if (line.getLevel() >= level) {
		cout << formatter(line.getMessage()) << endl;
	}
}

}
}


