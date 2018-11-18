#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include "util/io/ReadStream.hpp"

#include <stdio.h>

namespace Util {
namespace IO {
namespace FS {
class File;

class FileReadStream : public Util::IO::ReadStream
{
private:
	FILE* handle;

	FileReadStream(FILE* file);
public:
	virtual ~FileReadStream();

	// Stream interface
	virtual Result<Unit, ReadStream::Error> close();
	virtual Result<Unit, ReadStream::Error> read(uint8_t&);

	friend class File;
};

}
}
}


#endif // FILESTREAM_HPP
