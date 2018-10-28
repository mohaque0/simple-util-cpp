#ifndef ILLEGALSTATEEXCEPTION_HPP
#define ILLEGALSTATEEXCEPTION_HPP

#include "util/exception/Exception.hpp"

namespace Util {
namespace Exception {

class IllegalStateException : public Util::Exception::Exception
{
public:
	IllegalStateException(const char* msg) : Util::Exception::Exception(msg) {}
    ~IllegalStateException() noexcept (true) {}
};

}
}

#endif // ILLEGALSTATEEXCEPTION_HPP
