#include "ReadStream.hpp"

#include "util/io/ResizingBuffer.hpp"

namespace Util {
namespace IO {

ReadStream::ReadStream()
{}

ReadStream::~ReadStream()
{}

Result<Unit, ReadStream::Error> ReadStream::close()
{
	return result_ok<Unit, ReadStream::Error>(Unit());
}

Result<Unit, ReadStream::Error> ReadStream::readFull(ResizingBuffer& buffer)
{
	auto result = result_ok<Unit, ReadStream::Error>(Unit());
	uint8_t byte;
	while ((result = read(byte))) {
		buffer.put(&byte, 1);
	}
	return result;
}

Result<Unit, ReadStream::Error> ReadStream::readLine(ResizingBuffer& buffer)
{
	auto result = result_ok<Unit, ReadStream::Error>(Unit());
	uint8_t byte;
	while ((result = read(byte))) {
		if (byte == '\n') {
			break;
		}
		buffer.put(&byte, 1);
	}
	return result;
}

}
}

