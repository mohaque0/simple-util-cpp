#include "FileReadStream.hpp"

#include "system/Error.hpp"
#include "util/io/fs/File.hpp"

#include <string.h>

#define ERROR_BUFFER_SIZE 1024

namespace Util {
namespace IO {
namespace FS {

FileReadStream::FileReadStream(const File& file) :
	handle(NULL)
{
	const String pathString = file.getPath().getString();
	handle = fopen(pathString.c_str().get(), "r");
	if (handle == NULL) {
		printf("%s\n", pathString.c_str().get());
		throw Exception::IOException(("Error reading file: " + pathString).c_str().get());
	}
}

FileReadStream::~FileReadStream()
{
	close();
}

void FileReadStream::close()
{
	if (handle != NULL) {
		if (fclose(handle) != 0) {
			// Error
			char buffer[ERROR_BUFFER_SIZE];
			if (strerror_p(errno, buffer, ERROR_BUFFER_SIZE) != 0) {
				throw Exception::IOException("Unknown IO Error.");
			}
			throw Exception::IOException(buffer);
		}
		handle = NULL;
	}
}

ReadStream::Result FileReadStream::read(uint8_t& buffer)
{
	int nread = fread(&buffer, sizeof(uint8_t), 1, handle);
	if (nread < 1) {
		if (feof(handle) > 0) {
			return Result::EndOfStream;
		} else {
			return Result::Error;
		}
	}
	return Result::Ok;
}

}
}
}
