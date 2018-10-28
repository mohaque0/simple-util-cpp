#ifndef IOEXCEPTION_HPP
#define IOEXCEPTION_HPP

#include "util/exception/Exception.hpp"

namespace Util {
namespace Exception {

class IOException : public Util::Exception::Exception
{
public:
	IOException(const char* msg) : Util::Exception::Exception(msg) {}
	~IOException() noexcept (true) {}
};

}
}

#endif // IOEXCEPTION_HPP
