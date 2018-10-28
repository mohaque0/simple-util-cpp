#ifndef DEFAULTLOGFORMATTER_HPP
#define DEFAULTLOGFORMATTER_HPP

#include "util/logging/LogFormatter.hpp"

namespace Util {
namespace Logging {

class DefaultLogFormatter : public LogFormatter
{
public:
	DefaultLogFormatter();

	// LogFormatter interface
	const String operator ()(const String &msg) const;
};

}
}


#endif // DEFAULTLOGFORMATTER_HPP
