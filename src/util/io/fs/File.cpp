#include "File.hpp"

#include <stdio.h>

namespace Util {
namespace IO {
namespace FS {

File::File(const String &pathString) :
    path(Path(pathString))
{}

File::File(const Path& path_) :
	path(path_)
{}

const Path& File::getPath() const
{
	return path;
}

Result<FileReadStream, ReadStream::Error> File::stream() const
{
	const String pathString = getPath().getString();
	FILE* handle = fopen(pathString.c_str().get(), "r");
	if (handle == NULL) {
		return result_err<FileReadStream, ReadStream::Error>(ReadStream::Error::Unknown);
	}
	return result_ok<FileReadStream, ReadStream::Error>(FileReadStream(handle));
}

}
}
}
