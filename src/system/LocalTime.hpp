#ifndef LOCALTIME_HPP
#define LOCALTIME_HPP

#include "system/Platform.hpp"
#include <ctime>

#ifdef PLATFORM_WINDOWS
#define localtime_ts(l,t) localtime_s((l),(t))
#else
#define localtime_ts(l,t) localtime_r((t), (l))
#endif

#endif // LOCALTIME_HPP
