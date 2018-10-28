#ifndef STACKTRACE_HPP
#define STACKTRACE_HPP

#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include <cstdlib>
#include <string>

namespace System {

class StackFramePointer {
	typedef void* StackAddress;

	friend class StackTrace;

	StackAddress *ptr;
	char **str_ptr;
	StackFramePointer(StackAddress *ptr_, char **string_);

public:
	std::string getString();
	StackFramePointer operator++();
	std::string operator*();
	bool operator==(const StackFramePointer &other) const;
	bool operator!=(const StackFramePointer &other) const;
};

class StackTrace
{
	typedef void* StackAddress;

	StackAddress *buffer;
	char **strings;
	size_t size;

public:
    StackTrace();
	~StackTrace();
	StackFramePointer begin();
	StackFramePointer end();
};

}

#elif defined(PLATFORM_WINDOWS)

#include <string>

namespace System {

class StackFramePointer {
	typedef void* StackAddress;

	friend class StackTrace;

	StackAddress *ptr;
	char **str_ptr;
	StackFramePointer(StackAddress *ptr_, char **string_);

public:
	std::string getString();
	StackFramePointer operator++();
	std::string operator*();
	bool operator==(const StackFramePointer &other) const;
	bool operator!=(const StackFramePointer &other) const;
};

class StackTrace
{
	typedef void* StackAddress;

	StackAddress *buffer;
	char **strings;
	size_t size;

public:
    StackTrace();
	~StackTrace();
	StackFramePointer begin();
	StackFramePointer end();
};

}

#endif

#endif // STACKTRACE_HPP
