#ifndef Array3d_HPP
#define Array3d_HPP

#include "util/exception/IllegalStateException.hpp"

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

namespace Util {

template<typename T, class Allocator = std::allocator<T>>
class Array3d
{
	Allocator allocator;
	size_t size0_;
	size_t size1_;
    size_t size2_;
	T* data_;

	inline void copy(
		T* dst,
		const T* src,
		size_t n
	) {
		for (size_t i = 0; i < n; i++) {
			allocator.construct(&dst[i], src[i]);
		}
	}

	inline void moveData(
		T* dst,
		T* src,
		size_t n
	) {
		for (size_t i = 0; i < n; i++) {
			dst[i] = std::move(src[i]);
		}
	}

	/**
	 * @brief indexOf Returns the index of the data for (i0,i1) in the
	 * data array.
     *
     * @param s1 Each element of the second index is conceptually an array of size s2. This the size of that list of arrays.
     * @param s2 Each element of the third index is conceptually an element of type T. This the size of that list of elements.
     * @param i0 Index into array of 2d arrays of size s0.
     * @param i1 Index into array of arrays of size s1.
     * @param i2 Index into array of size s2.
	 * @return
	 */
	inline size_t indexOf(
        const size_t s1,
        const size_t s2,
		const size_t i0,
        const size_t i1,
        const size_t i2
	) const {
		return (s1*s2) * i0 + s2 * i1 + i2;
	}

	void resize(const size_t s0, const size_t s1, const size_t s2) {
		if (s0 == size0_ && s1 == size1_ && s2 == size2_) {
			return;
		}

		T* tmp = allocator.allocate(s0 * s1 * s2);
		const size_t minSize0 = (size0_ < s0) ? size0_ : s0;
		const size_t minSize1 = (size1_ < s1) ? size1_ : s1;
		const size_t minSize2 = (size2_ < s2) ? size2_ : s2;

		// It only makes sense to call moveData on a contiguous set of entries.
		// Rather than iterating over the last index we copy it.
		if (data_ != NULL) {
			for (size_t i0 = 0; i0 < minSize0; i0++) {
				for (size_t i1 = 0; i1 < minSize1; i1++) {
					moveData(
						&tmp[indexOf(s1, s2, i0, i1, 0)],
						&data_[indexOf(size1_, size2_, i0, i1, 0)],
						minSize2
					);
				}
			}
			allocator.deallocate(data_, size0_ * size1_ * size2_);
		}

		data_ = tmp;
		size0_ = s0;
		size1_ = s1;
		size2_ = s2;
	}


public:
    Array3d() :
		size0_(0),
		size1_(0),
        size2_(0),
		data_(NULL)
	{}
    Array3d(const size_t s0, const size_t s1, const size_t s2) :
		size0_(0),
		size1_(0),
		size2_(0),
		data_(NULL)
	{
		resizeAndEmplaceNewEntries(s0, s1, s2);
	}
    Array3d(Array3d&& other) :
		size0_(other.size0()),
		size1_(other.size1()),
        size2_(other.size2()),
		data_(other.data_)
	{
		other.size0_ = 0;
		other.size1_ = 0;
		other.size2_ = 0;
		other.data_ = NULL;
	}
    Array3d(const Array3d &other) :
		size0_(other.size0()),
		size1_(other.size1()),
        size2_(other.size2()),
        data_(allocator.allocate(size0_ * size1_ * size2_))
	{
        copy(data_, other.data_, size0_ * size1_ * size2_);
	}

    virtual ~Array3d() {
		if (data_ != NULL) {
			for (size_t i0 = 0; i0 < size0_; i0++) {
				for (size_t i1 = 0; i1 < size1_; i1++) {
					for (size_t i2 = 0; i2 < size2_; i2++) {
						allocator.destroy(&at(i0, i1, i2));
					}
				}
			}
			allocator.deallocate(data_, size0_ * size1_ * size2_);
		}
	}

	size_t size0() const {
		return size0_;
	}

	size_t size1() const {
		return size1_;
	}

	size_t size2() const {
        return size2_;
    }

    T& at(const size_t i0, const size_t i1, const size_t i2) {
        return data_[indexOf(size1_, size2_, i0, i1, i2)];
	}

    const T& at(const size_t i0, const size_t i1, const size_t i2) const {
        return data_[indexOf(size1_, size2_, i0, i1, i2)];
    }

	template <class... Args>
	void emplace(const size_t i0, const size_t i1, const size_t i2, Args&&... args) {
		if (data_ == NULL) {
			throw Exception::IllegalStateException("Attempting to access data in an empty array.");
		}

		T* ptr = &at(i0, i1, i2);
		allocator.destroy(ptr);
		allocator.construct(ptr, std::forward<Args>(args)...);
	}

	void set(T&& value, const size_t i0, const size_t i1, const size_t i2) {
		if (data_ == NULL) {
			throw Exception::IllegalStateException("Attempting to access data in an empty array.");
		}

		at(i0, i1, i2) = std::move(value);
	}

	void set(const T& value, const size_t i0, const size_t i1, const size_t i2) {
		at(i0, i1, i2) = value;
	}

	void setAll(const T& value) {
		const size_t max_index = size0_ * size1_ * size2_;
		for (size_t i = 0; i < max_index; i++) {
			data_[i] = value;
		}
	}

	template <class... Args>
	void resizeAndEmplaceNewEntries(const size_t s0, const size_t s1, const size_t s2, Args... args) {
		const size_t prev_s0 = size0_;
		const size_t prev_s1 = size1_;
		const size_t prev_s2 = size2_;

		resize(s0, s1, s2);

		// TODO: This could be more efficient.
		for (size_t i0 = 0; i0 < s0; i0++) {
			for (size_t i1 = 0; i1 < s1; i1++) {
				for (size_t i2 = 0; i2 < s2; i2++) {
					if (i0 >= prev_s0 || i1 >= prev_s1 || i2 >= prev_s2) {
						allocator.construct(&at(i0, i1, i2), std::forward<Args>(args)...);
					}
				}
			}
		}
	}

	/**
	 * @brief resizeAndSetNewEntries This will resize the data and set any new indices to the given value.
	 * The value is set by calling the copy constructor.
	 * The old indicies will still point to their previous values.
	 *
	 * @param s0 New size for dimension 0.
	 * @param s1 New size for dimension 1.
	 * @param s2 New size for dimension 2.
	 * @param value Value to set new entries to.
	 */
	void resizeAndSetNewEntries(const size_t s0, const size_t s1, const size_t s2, const T& value) {
		resizeAndEmplaceNewEntries(s0, s1, s2, value);
	}

	const T* data() const {
		return data_;
	}

};

}

#endif // Array3d_HPP
