#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

namespace Util {
namespace Exception {

class Exception : std::exception
{	
private:
	std::string msg;

public:
	Exception(const char* msg_);
	virtual ~Exception() noexcept (true) {}

	virtual const char* what() const throw() {
		return msg.c_str();
	}
};

}
}

#endif // EXCEPTION_HPP
