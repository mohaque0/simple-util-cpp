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

public:
	FileReadStream(const File& file) throw(Exception::IOException);
	virtual ~FileReadStream();

	// Stream interface
	virtual void close() throw(Exception::IOException);
	virtual Result read(uint8_t&);
};

}
}
}


#endif // FILESTREAM_HPP
