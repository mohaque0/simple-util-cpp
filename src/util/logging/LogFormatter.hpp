#ifndef LOGFORMATTER_HPP
#define LOGFORMATTER_HPP

#include "util/String.hpp"

namespace Util {
namespace Logging {

class LogFormatter
{
public:
	LogFormatter();
	virtual const String operator()(const String &msg) const = 0;
};

}
}


#endif // LOGFORMATTER_HPP
