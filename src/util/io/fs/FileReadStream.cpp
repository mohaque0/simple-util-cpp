#include "FileReadStream.hpp"

#include "system/Error.hpp"
#include "util/io/fs/File.hpp"

#include <string.h>

#define ERROR_BUFFER_SIZE 1024

namespace Util {
namespace IO {
namespace FS {

FileReadStream::FileReadStream(FILE* file) :
	handle(file)
{}

FileReadStream::~FileReadStream()
{
	close();
}

Result<Unit, ReadStream::Error> FileReadStream::close()
{
	if (handle != NULL) {
		if (fclose(handle) != 0) {
			return result_err<Unit>(ReadStream::Error::Unknown);
		}
		handle = NULL;
	}

	return result_ok<Unit, ReadStream::Error>(Unit());
}

Result<Unit, ReadStream::Error> FileReadStream::read(uint8_t& buffer)
{
	size_t nread = fread(&buffer, sizeof(uint8_t), 1, handle);
	if (nread < 1) {
		if (feof(handle) > 0) {
			return result_err<Unit>(ReadStream::Error::EndOfStream);
		} else {
			return result_err<Unit>(ReadStream::Error::Unknown);;
		}
	}
	return result_ok<Unit, ReadStream::Error>(Unit());
}

}
}
}
