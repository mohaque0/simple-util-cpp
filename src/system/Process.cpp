#include "Process.hpp"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

#include "util/exception/IllegalStateException.hpp"
#include "util/exception/IllegalArgumentException.hpp"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace System {

namespace {

// TODO: This should be somewhere else.
bool fileExists(const char *filename) {
	if (filename == NULL) {
		return false;
	}

	struct stat filestats;
	return stat(filename, &filestats) == 0;
}

}

Process::Process(const char * const cmd[]) :
	pid(0),
	statusData(0),
	didGetStatus(false)
{
	if (pipe(in.address()) != 0) {
		throw Util::Exception::IllegalStateException(strerror(errno));
	}
	if (pipe(out.address()) != 0) {
		throw Util::Exception::IllegalStateException(strerror(errno));
	}
	if (pipe(err.address()) != 0) {
		throw Util::Exception::IllegalStateException(strerror(errno));
	}
	if (cmd[0] == NULL) {
		throw Util::Exception::IllegalArgumentException("Command arguments must include command name as the first argument.");
	}
	if (!fileExists(cmd[0])) {
		throw Util::Exception::IllegalStateException("Executable does not exist.");
	}

	pid = fork();
	if (pid != 0) {
		// Parent Process
		close(in.readablefd());
		close(out.writeablefd());
		close(err.writeablefd());

	} else {
		// Child Process
		dup2(in.readablefd(), STDIN_FILENO);
		dup2(out.writeablefd(), STDOUT_FILENO);
		dup2(err.writeablefd(), STDERR_FILENO);

		void *ptr = (void *)cmd;

		// Pointer hackery to make the types work.
		execv(cmd[0], (char * const *)ptr);

		throw Util::Exception::IllegalStateException(strerror(errno));
	}
}

Process::~Process()
{
	close(in.writeablefd());
	close(out.readablefd());
	close(err.readablefd());
}

int Process::getPid()
{
	return pid;
}

int Process::getStdinFd()
{
	return in.writeablefd();
}

int Process::getStdoutFd()
{
	return out.readablefd();
}

int Process::getStderrFd()
{
	return err.readablefd();
}

// Note: getStatus() depends on this setting the status.
bool Process::isDone()
{
	if (didGetStatus) {
		return true;
	}

	int ret = waitpid(pid, &statusData, WNOHANG);
	if (ret == 0) {
		return false;
	} else if (ret == -1) {
		// Error
		throw std::exception();
	} else {
		return true;
	}
}

int Process::waitForExit()
{
	int ret = waitpid(pid, &statusData, 0);
	if (ret == -1) {
		throw std::exception();
	}
	didGetStatus = true;
	printf("r=%d s=%d\n", ret, statusData);
	return getStatus();
}

int Process::getStatus()
{
	// This also sets the status.
	isDone();
	return WEXITSTATUS(statusData);
}

}

#elif defined(PLATFORM_WINDOWS)


#endif