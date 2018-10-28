#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

#include "util/exception/IllegalStateException.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace Util {

template <class T, class Allocator = std::allocator<T>>
class CircularBuffer
{
private:
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
	void pushEndIndex() {
		// Out of space.
		if (hasValues && end == start) {
			throw Exception::IllegalStateException("Out of space.");
		}

		end++;
		if (end >= capacity_) {
			end = 0;
		}
		hasValues = true;
	}

	/**
	 * Move the start index up by one.
	 */
	void pushStartIndex() {
		// No more elements.
		if (!hasValues && start == end) {
			throw Exception::IllegalStateException("No more elements.");
		}

		start++;
		if (start >= capacity_) {
			start = 0;
		}
		if (start == end) {
			hasValues = false;
		}
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

	void pop_front() {
		T * const ptr = buffer + start;
		allocator.destroy(ptr);
		pushStartIndex();
	}

    T& peek_front() {
        return *(buffer + start);
    }

    const T& peek_front() const {
        return *(buffer + start);
    }

	template <class... Args>
	T& emplace_back(Args&&... args) {
		T * const ptr = buffer + end;
		allocator.construct(ptr, std::forward<Args>(args)...);
		pushEndIndex();
		return *ptr;
	}

};

}

#endif // CIRCULARBUFFER_HPP
