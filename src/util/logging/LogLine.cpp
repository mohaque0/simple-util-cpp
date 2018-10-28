#include "LogLine.hpp"

#include <memory>

namespace Util {
namespace Logging {

LogLine::LogLine(LogLevel level_, String&& message_) :
	level(level_),
	message(std::move(message_))
{}

const LogLevel LogLine::getLevel() const
{
	return level;
}

const String LogLine::getMessage() const
{
	return message;
}

}
}
