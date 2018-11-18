#ifndef ARRAY2D_HPP
#define ARRAY2D_HPP

#include "util/Array3d.hpp"

#include <cstddef>
#include <cstring>
#include <cmath>
#include <memory>
#include <utility>

namespace Util {

enum class Array2dError {
	OutOfBounds
};

template<typename T, class Allocator = std::allocator<T>>
class Array2d
{
	Array3d<T,Allocator> container;

public:
	Array2d() :
		container()
	{}
	Array2d(size_t s0, size_t s1) :
		container(s0,s1,1)
	{
		resizeAndEmplaceNewEntries(s0, s1);
	}
	Array2d(Array2d&& other) :
		container(std::move(other.container))
	{}
	Array2d(const Array2d &other) :
		container(other.container)
	{}

	virtual ~Array2d() {}

	size_t size0() const {
		return container.size0();
	}

	size_t size1() const {
		return container.size1();
	}

	T& at(size_t i0, size_t i1) {
		return container.at(i0,i1,0);
	}

	const T& at(size_t i0, size_t i1) const {
		return container.at(i0,i1,0);
	}

	template <class... Args>
	Result<T*, Array2dError> emplace(const size_t i0, const size_t i1, Args&&... args) {
		auto result = container.emplace(i0, i1, 0, std::forward<Args>(args)...);

		if (result) {
			return result_ok<T*, Array2dError>(result.ok());
		} else {
			switch (result.error()) {
			case Array3dError::OutOfBounds: {
				return result_err<T*,Array2dError>(Array2dError::OutOfBounds);
				break;
			}
			}

			throw std::runtime_error("Unreachable.");
		}
	}

	template <class... Args>
	void resizeAndEmplaceNewEntries(const size_t newSize0, const size_t newSize1, Args... args) {
		container.resizeAndEmplaceNewEntries(newSize0, newSize1, 1, std::forward<Args>(args)...);
	}

	const T* data() const {
		return container.data();
	}

};

}

#endif // ARRAY2D_HPP
