#ifndef ILLEGALARGUMENTEXCEPTION_HPP
#define ILLEGALARGUMENTEXCEPTION_HPP

#include "util/exception/Exception.hpp"

namespace Util {
namespace Exception {

class IllegalArgumentException : public Util::Exception::Exception
{
public:
	IllegalArgumentException(const char* msg) : Util::Exception::Exception(msg) {}
	~IllegalArgumentException() noexcept (true) {}
};

}
}

#endif // ILLEGALARGUMENTEXCEPTION_HPP
