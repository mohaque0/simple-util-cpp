#include "StackTrace.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include "system/Process.hpp"

#include <cxxabi.h>
#include <errno.h>
#include <execinfo.h>
#include <limits.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define INIT_BUFFER_SIZE 10
#define MAX_CHARS_FOR_PTR 20
#define MAX_CHARS_FOR_EXE (PATH_MAX+FILENAME_MAX)

namespace System {

namespace {

typedef void* StackAddress;

// TODO: This should be somewhere else.
bool fileExists(const char *filename) {
	if (filename == NULL) {
		return false;
	}

	struct stat filestats;
	return stat(filename, &filestats) == 0;
}

// TODO: This belongs somewhere else.
std::string fd2string(int fd) {
		char buffer[100];
		int nread = 0;
		std::string str = "";
		while ((nread = read(fd, buffer, 100)) > 0) {
			str += std::string(buffer, 0, nread);
		}

		// Trim the newline.
		if (str.size() > 0) {
			str = str.substr(0, str.size()-1);
		}

		return str;
}

/**
 * @brief addr2line Converts an address in memory to a stacktrace line.
 *
 * This is an ugly hack. Sorry.
 *
 * @param addr
 * @return
 */
std::string addr2line(StackAddress addr) {
	// TODO: Must determine the correct location of addr2line.
	const char *proc = "/usr/bin/addr2line";
	char *addrstr;
	char *exestr;

	// For 64-bits, it should never take more than this many characters
	addrstr = (char *)malloc(MAX_CHARS_FOR_PTR * sizeof(char));
	if (addrstr == NULL) {
		return std::string();
	}

	exestr = (char *)malloc(MAX_CHARS_FOR_EXE * sizeof(char));
	if (exestr == NULL) {
		free(addrstr);
		return std::string();
	}

	// Turn the address into a string.
	snprintf(addrstr, MAX_CHARS_FOR_PTR, "%p", addr);

	// Get the name of the executable binary.
	int nwrit = readlink("/proc/self/exe", exestr, MAX_CHARS_FOR_EXE-1);
	if (nwrit == -1 || nwrit >= MAX_CHARS_FOR_EXE) {
		free(addrstr);
		free(exestr);
		return std::string();
	} else {
		// Null terminate the string.
		exestr[nwrit] = '\0';
	}

	// Make a pipe to read output from process.
	int outpipe[2] = {0,0};
	if (pipe(outpipe) !=0) {
		free(addrstr);
		free(exestr);
		return std::string();
	}

	int pid = fork();
	if (pid == -1) {
		free(addrstr);
		free(exestr);
		return std::string();

	} else if (pid != 0) {
		// Parent.
		close(outpipe[1]);

	} else {
		// Child.
		// The child always exists with status 1 if anything bad happens.

		// Make the stdout file descriptor point to the pipe.
		if (dup2(outpipe[1], STDOUT_FILENO) == -1) {
			exit(1);
		}

		execlp(proc, proc, addrstr, "-e", exestr, NULL);

		// If this happened something failed.
		exit(1);
	}

	// Don't need these anymore.
	free(addrstr);
	free(exestr);

	int statusData = 0;
	int ret = waitpid(pid, &statusData, 0);
	if (ret == -1) {
		printf("Interrupted.\n");
		return std::string();
	}
	if (WEXITSTATUS(statusData) != 0) {
		printf("Bad status.\n");
		return std::string();
	}

	std::string addrline(fd2string(outpipe[0]));

	// Trim the strings if necessary.
	size_t useful_loc = addrline.find("/src/");
	if (useful_loc != std::string::npos) {
		addrline = addrline.substr(useful_loc+5);
	}

	close(outpipe[0]);

	return addrline;
}

std::string filterLineString(std::string line) {
	size_t end = line.find("(discriminator");
	if (end != std::string::npos) {
		line = line.substr(0, end-1);
	}

	return line;
}

std::string filterAddrString(char *addrStrChars) {
	std::string addrString(addrStrChars);

	size_t start = addrString.find("(");
	if (start == std::string::npos) {
		return addrString;
	}

	size_t end = addrString.find("+", start+1);
	if (end == std::string::npos || end <= start+1) {
		return addrString;
	}

	addrString = addrString.substr(start+1, end-start-1);
	int status = 0;
	char *demangledName = abi::__cxa_demangle(addrString.c_str(), NULL, NULL, &status);
	if (demangledName != NULL) {
		addrString = std::string(demangledName);
		free(demangledName);
	}

	return addrString;
}

}


StackFramePointer::StackFramePointer(StackAddress *ptr_, char **string_) :
	ptr(ptr_),
	str_ptr(string_)
{}

std::string StackFramePointer::getString()
{
	std::string lineString = filterLineString(addr2line(*ptr));
	std::string addrString = filterAddrString(*str_ptr);

	return addrString + " (" + lineString + ")";
}

StackFramePointer StackFramePointer::operator++()
{
	return StackFramePointer(++ptr, ++str_ptr);
}

std::string StackFramePointer::operator*()
{
	return getString();
}

bool StackFramePointer::operator==(const StackFramePointer &other) const
{
	// This check should be sufficient.
	return str_ptr == other.str_ptr;
}

bool StackFramePointer::operator!=(const StackFramePointer &other) const
{
	return !(*this == other);
}


StackTrace::StackTrace() :
	buffer(0),
	strings(0),
	size(0)
{
	size_t buffer_size = INIT_BUFFER_SIZE;
	buffer = (StackAddress *)malloc(buffer_size * sizeof(StackAddress));

	while (buffer != NULL && (size = backtrace(buffer, buffer_size)) >= buffer_size) {
		size_t newBufferSize = buffer_size * 2;
		StackAddress *newBuffer = (StackAddress*)realloc((void*)buffer, newBufferSize * sizeof(StackAddress));
		if (newBuffer != NULL) {
			buffer_size = newBufferSize;
			buffer = newBuffer;
		} else {
			// Failed to allocate space.
			break;
		}
	}

	if (buffer == NULL) {
		return;
	}

	strings = backtrace_symbols(buffer, size);
}

StackTrace::~StackTrace()
{
	if (buffer != NULL) {
		free(buffer);
	}
	if (strings != NULL) {
		free(strings);
	}
}

StackFramePointer StackTrace::begin()
{
	return StackFramePointer(buffer, strings);
}

StackFramePointer StackTrace::end()
{
	return StackFramePointer(&buffer[size], &strings[size]);
}

}

#elif defined(PLATFORM_WINDOWS)

#include <errno.h>
#include <limits.h>
#include <string>
#include <sys/stat.h>
#include <DbgEng.h>
#include <Windows.h>

#define INIT_BUFFER_SIZE 10
#define MAX_CHARS_FOR_PTR 20
#define MAX_CHARS_FOR_EXE (PATH_MAX+FILENAME_MAX)

namespace System {

namespace {

typedef void* StackAddress;

}

StackFramePointer::StackFramePointer(StackAddress *ptr_, char **string_) :
	ptr(ptr_),
	str_ptr(string_)
{}

std::string StackFramePointer::getString()
{
	// TODO: Unimplemented.
	// Implementation would require SymInitialize and SymFromAddr
	return "Unknown (unknown)";
}

StackFramePointer StackFramePointer::operator++()
{
	return StackFramePointer(++ptr, ++str_ptr);
}

std::string StackFramePointer::operator*()
{
	return getString();
}

bool StackFramePointer::operator==(const StackFramePointer &other) const
{
	// This check should be sufficient.
	return str_ptr == other.str_ptr;
}

bool StackFramePointer::operator!=(const StackFramePointer &other) const
{
	return !(*this == other);
}


StackTrace::StackTrace() :
	buffer(0),
	strings(0),
	size(0)
{
	size_t buffer_size = INIT_BUFFER_SIZE;
	buffer = (StackAddress *)malloc(buffer_size * sizeof(StackAddress));

	while (buffer != NULL && (size = CaptureStackBackTrace(0, buffer_size, buffer, NULL)) >= buffer_size) {
		size_t newBufferSize = buffer_size * 2;
		StackAddress *newBuffer = (StackAddress*)realloc((void*)buffer, newBufferSize * sizeof(StackAddress));
		if (newBuffer != NULL) {
			buffer_size = newBufferSize;
			buffer = newBuffer;
		} else {
			// Failed to allocate space.
			break;
		}
	}

	if (buffer == NULL) {
		return;
	}

	// TODO: Unimplemented.
	strings = nullptr;
}

StackTrace::~StackTrace()
{
	if (buffer != NULL) {
		free(buffer);
	}
	if (strings != NULL) {
		free(strings);
	}
}

StackFramePointer StackTrace::begin()
{
	return StackFramePointer(buffer, strings);
}

StackFramePointer StackTrace::end()
{
	return StackFramePointer(&buffer[size], &strings[size]);
}

}

#endif
