#include "LogLevel.hpp"

#define LOG_LEVEL_ERROR   4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_DEBUG   1
#define LOG_LEVEL_TRACE   0

namespace Util {
namespace Logging {

LogLevel::LogLevel(int level_) :
level(level_)
{}

bool LogLevel::operator==(const LogLevel &other) const
{
	return level == other.level;
}

bool LogLevel::operator!=(const LogLevel &other) const
{
	return level != other.level;
}

bool LogLevel::operator<(const LogLevel &other) const
{
	return level < other.level;
}

bool LogLevel::operator<=(const LogLevel &other) const
{
	return level <= other.level;
}

bool LogLevel::operator>(const LogLevel &other) const
{
	return level > other.level;
}

bool LogLevel::operator>=(const LogLevel &other) const
{
	return level >= other.level;
}

LogLevel LogLevel::Error() {
	return LogLevel(LOG_LEVEL_ERROR);
}

LogLevel LogLevel::Warning() {
	return LogLevel(LOG_LEVEL_WARNING);
}

LogLevel LogLevel::Info() {
	return LogLevel(LOG_LEVEL_INFO);
}

LogLevel LogLevel::Debug() {
	return LogLevel(LOG_LEVEL_DEBUG);
}

LogLevel LogLevel::Trace() {
	return LogLevel(LOG_LEVEL_TRACE);
}

}
}
