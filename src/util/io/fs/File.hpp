#ifndef FILE_HPP
#define FILE_HPP

#include "util/io/Path.hpp"

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
};

}
}
}


#endif // FILE_HPP
