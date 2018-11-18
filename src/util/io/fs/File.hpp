#ifndef FILE_HPP
#define FILE_HPP

#include "util/io/Path.hpp"
#include "util/io/fs/FileReadStream.hpp"
#include "util/Result.hpp"

namespace Util {
namespace IO {
namespace FS {

class File
{
	const Path path;
public:
    File(const String& pathString);
	File(const Path& path);
	const Path& getPath() const;
	Result<FileReadStream, ReadStream::Error> stream() const;
};

}
}
}


#endif // FILE_HPP
