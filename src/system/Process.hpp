#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "system/Platform.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

namespace System {

class Pipe {
	int pipe[2];

public:
	Pipe() : pipe {0,0} {}
	inline int *address() { return pipe; }
	inline int readablefd() const { return pipe[0]; }
	inline int writeablefd() const { return pipe[1]; }
};

class Process
{
private:
	int pid;

	// Pipe for the process's stdin. Writable to the caller.
	Pipe in;

	// Pipe for the process's stdout. Readable from the caller.
	Pipe out;

	// Pipe for the process's stderr. Readable from the caller.
	Pipe err;

	// This holds the data from waitpid, but according to the docs it is not
	// the same as the return status of the process.
	int statusData;

	// This is set lazily upon request.
	bool didGetStatus;

public:
	Process(const char * const cmd[]);
	~Process();
	int getPid();
	int getStdinFd();
	int getStdoutFd();
	int getStderrFd();

	bool isDone();

	/**
	 * @brief waitForExit
	 *
	 * Wait for the process to exit.
	 *
	 * @return Exit status of the child process.
	 */
	int waitForExit();
	int getStatus();
};

}

#elif defined(PLATFORM_WINDOWS)

namespace System {

class Process
{
private:

public:
	Process(const char * const cmd[]);
	~Process();
	int getPid();
	int getStdinFd();
	int getStdoutFd();
	int getStderrFd();

	bool isDone();

	/**
	 * @brief waitForExit
	 *
	 * Wait for the process to exit.
	 *
	 * @return Exit status of the child process.
	 */
	int waitForExit();
	int getStatus();
};

}

#endif

#endif // PROCESS_HPP
