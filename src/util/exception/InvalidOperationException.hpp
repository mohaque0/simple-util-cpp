#ifndef INVALIDOPERATIONEXCEPTION_H
#define INVALIDOPERATIONEXCEPTION_H

#include "util/exception/Exception.hpp"

namespace Util {
namespace Exception {

class InvalidOperationException : public Util::Exception::Exception
{
public:
	InvalidOperationException(const char* msg) : Util::Exception::Exception(msg) {}
    ~InvalidOperationException() noexcept (true) {}
};

}
}

#endif // INVALIDOPERATIONEXCEPTION_H
