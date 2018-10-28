#ifndef ERROR_HPP
#define ERROR_HPP

#include "system/Platform.hpp"

#include <string.h>

#ifdef PLATFORM_MAC
#include <sys/errno.h>
#endif
#ifdef PLATFORM_LINUX
#include <error.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define strerror_p(errno, buf, size) strerror_s((buf), (size), (errno))
#else
#define strerror_p(errno, buf, size) strerror_r((errno), (buf), (size))
#endif

#endif // ERROR_HPP

