#include "DefaultLogFormatter.hpp"

#include "util/Time.hpp"

namespace Util {
namespace Logging {

DefaultLogFormatter::DefaultLogFormatter()
{}

const String DefaultLogFormatter::operator ()(const String &msg) const
{
	return "[" + Time::getCurrentTime().toLocalString() + "] " + msg;
}



}
}
