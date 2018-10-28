#ifndef LOGLINE_HPP
#define LOGLINE_HPP

#include "util/String.hpp"
#include "util/logging/LogLevel.hpp"

namespace Util {
namespace Logging {

class LogLine
{
	LogLevel level;
	String message;
public:
	LogLine(LogLevel level_, String&& message_);
	const LogLevel getLevel() const;
	const String getMessage() const;
};

}
}


#endif // LOGLINE_HPP
