#ifndef LOGLEVEL_HPP
#define LOGLEVEL_HPP

namespace Util {
namespace Logging {

class LogLevel
{
	int level;
	LogLevel(int level_);

public:
	bool operator==(const LogLevel &other) const;
	bool operator!=(const LogLevel &other) const;
	bool operator<(const LogLevel &other) const;
	bool operator<=(const LogLevel &other) const;
	bool operator>(const LogLevel &other) const;
	bool operator>=(const LogLevel &other) const;

	static LogLevel Error();
	static LogLevel Warning();
	static LogLevel Info();
	static LogLevel Debug();
	static LogLevel Trace();
};

}
}


#endif // LOGLEVEL_HPP
