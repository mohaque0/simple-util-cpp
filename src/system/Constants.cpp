#include "Constants.hpp"

#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

namespace System {

const char* fileSeparator()
{
	return "/";
}

}

#elif defined(PLATFORM_WINDOWS)

namespace System {

const char* fileSeparator()
{
	return "\\";
}

}

#endif