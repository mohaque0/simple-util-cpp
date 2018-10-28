#include "Time.hpp"

#include "system/LocalTime.hpp"

#define BUFFER_SIZE 1024

namespace Util {

Time::Time(time_t t_) :
	t(t_)
{}

String Time::toLocalString(const char *format)
{
	// Temporary array to store output string.
	char buffer[BUFFER_SIZE];
	struct tm localizedTime;

	localtime_ts(&localizedTime, &t);
	size_t written = strftime(buffer, BUFFER_SIZE, format, &localizedTime);
	return String(buffer, 0, written);
}

Time Time::getCurrentTime()
{
	return Time(time(NULL));
}

}


