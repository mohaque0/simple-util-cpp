#include "Exception.hpp"

#include "system/StackTrace.hpp"

#include <iostream>

namespace Util {
namespace Exception {

namespace {

std::string buildStackString(const char* msg) {
	using namespace System;

	StackTrace stackTrace;
	std::string accumulatedFrames = std::string("Exception: ") + std::string(msg);

	for (std::string f : stackTrace) {
		accumulatedFrames = accumulatedFrames + "\n  " + f;
	}

	return accumulatedFrames;
}

}

Exception::Exception(const char *msg_) :
	msg(buildStackString(msg_))
{}

}
}

