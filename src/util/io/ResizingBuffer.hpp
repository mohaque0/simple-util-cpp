#ifndef RESIZINGBUFFER_HPP
#define RESIZINGBUFFER_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Util {
namespace IO {

/**
 * @brief The ResizingBuffer class is currently just a wrapper around
 * std::vector but may include some checks for max-size in the future.
 */
class ResizingBuffer
{
	std::vector<uint8_t> buffer;

public:
	void clear();
	const uint8_t* const get() const;
	void put(uint8_t* c, size_t size);
	void shrink_to_fit();
	const size_t size() const;
};

}
}

#endif // RESIZINGBUFFER_HPP
