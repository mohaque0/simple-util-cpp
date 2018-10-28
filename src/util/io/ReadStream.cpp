#include "ReadStream.hpp"

#include "util/io/ResizingBuffer.hpp"

using Util::Exception::IOException;

namespace Util {
namespace IO {

ReadStream::ReadStream()
{}

ReadStream::~ReadStream() throw(IOException)
{}

void ReadStream::close() throw(IOException)
{}

ReadStream::Result ReadStream::readFull(ResizingBuffer& buffer)
{
	Result result = Result::Ok;
	uint8_t byte;
	while ((result = read(byte)) == Result::Ok) {
		buffer.put(&byte, 1);
	}
	return result;
}

ReadStream::Result ReadStream::readLine(ResizingBuffer& buffer)
{
	Result result = Result::Ok;
	uint8_t byte;
	while ((result = read(byte)) == Result::Ok) {
		if (byte == '\n') {
			break;
		}
		buffer.put(&byte, 1);
	}
	return result;
}

}
}

