#include "ResizingBuffer.hpp"

namespace Util {
namespace IO {

void ResizingBuffer::clear()
{
	buffer.clear();
}

const uint8_t* const ResizingBuffer::get() const
{
	return buffer.data();
}

void ResizingBuffer::put(uint8_t* c, size_t size)
{
	buffer.reserve(buffer.size() + size);
	buffer.insert(buffer.end(), c, c+size);
}

void ResizingBuffer::shrink_to_fit()
{
	buffer.shrink_to_fit();
}

const size_t ResizingBuffer::size() const
{
	return buffer.size();
}

}
}
