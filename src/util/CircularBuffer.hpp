#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

#include "util/Result.hpp"
#include "util/Unit.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace Util {

enum class CircularBufferError {
	NoMoreElements,
	OutOfSpace
};

template <class T, class Allocator = std::allocator<T>>
class CircularBuffer
{
private:
	typedef CircularBufferError Error;

	Allocator allocator;
	T* buffer;
	size_t start;    // This is inclusive.
	size_t end;      // This is not inclusive.
	size_t capacity_;

	/**
	 * This is managed by size(), pushEnd() and pushStart()
	 */
	bool hasValues;

	/**
	 * Move the end index up by one.
	 */
	Result<Unit,Error> pushEndIndex() {
		// Out of space.
		if (hasValues && end == start) {
			return result_err<Unit,Error>(Error::OutOfSpace);
		}

		end++;
		if (end >= capacity_) {
			end = 0;
		}
		hasValues = true;

		return result_ok<Unit,Error>(Unit());
	}

	/**
	 * Move the start index up by one.
	 */
	Result<Unit,Error> pushStartIndex() {
		// No more elements.
		if (!hasValues && start == end) {
			return result_err<Unit,Error>(Error::NoMoreElements);
		}

		start++;
		if (start >= capacity_) {
			start = 0;
		}
		if (start == end) {
			hasValues = false;
		}

		return result_ok<Unit,Error>(Unit());
	}

public:
	CircularBuffer(const size_t _cap) :
		buffer(allocator.allocate(_cap)),
		start(0),
		end(0),
		capacity_(_cap),
		hasValues(false)
	{}

	virtual ~CircularBuffer() {
		allocator.deallocate(buffer, capacity_);
	}

	const size_t size() const {
		if (hasValues && end == start) {
			return capacity_;
		} else if (end < start) {
			return (capacity_ - start) + end;
		} else {
			return end - start;
		}
	}

	const size_t capacity() const {
		return capacity_;
	}

	Result<Unit,Error> pop_front() {
		T * const ptr = buffer + start;
		if (hasValues) {
			allocator.destroy(ptr);
		}
		return pushStartIndex();
	}

    T& peek_front() {
        return *(buffer + start);
    }

    const T& peek_front() const {
        return *(buffer + start);
    }

	template <class... Args>
	Result<Unit,Error> emplace_back(Args&&... args) {
		T * const ptr = buffer + end;
		if (size() < capacity()) {
			allocator.construct(ptr, std::forward<Args>(args)...);
		}
		return pushEndIndex();
	}

};

}

#endif // CIRCULARBUFFER_HPP
