#include "File.hpp"

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

}
}
}
