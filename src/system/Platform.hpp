/*
 * Only supports detection of Linux, Mac, and Windows.
 */

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef __APPLE__
#define PLATFORM_MAC
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#elif defined(__linux__)
#define PLATFORM_LINUX
#else
#error Unsupported platform.
#endif

#endif // PLATFORM_HPP