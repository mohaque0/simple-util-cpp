#ifndef LOGSINK_HPP
#define LOGSINK_HPP

namespace Util {
namespace Logging {

class LogLine;

class LogSink
{
public:
	LogSink();
	virtual ~LogSink();
	virtual void log(LogLine &line) = 0;
};

}
}


#endif // LOGSINK_HPP
