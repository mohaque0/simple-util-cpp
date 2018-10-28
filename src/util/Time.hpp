#ifndef TIME_HPP
#define TIME_HPP

#include "util/String.hpp"

#include <ctime>

namespace Util {

class Time
{
	const time_t t;
	Time(time_t t_);
public:
	/**
	 * @brief toLocalString
	 * @param format This accepts the same format strings as strftime().
	 * @return This time formatted as a string.
	 */
	String toLocalString(const char * format="%F %I:%M:%S %p");

	static Time getCurrentTime();
};



}

#endif // TIME_HPP
