#ifndef RESULT_HPP
#define RESULT_HPP

#include <algorithm>
#include <type_traits>

namespace Util {

// Prototypes (necessary for friending)

template <typename T, typename E>
class Result;

template <typename T, typename E>
static Result<T, E> result_ok(T&& value);

template <typename T, typename E>
static Result<T, E> result_err(E&& value);


// Implementation

template <typename T, typename E>
class Result
{
	// This guarantees we have aligned space large enough for both types.
	struct data_storage {
		alignas(std::max({alignof(T), alignof(E)})) uint8_t bytes[std::max({sizeof(T), sizeof(E)})];
	} data;
	bool errorFlag;

	// This should only be used by the static methods result_ok and result_err.
	Result() :
		errorFlag(false)
	{}

public:
	Result(T&& value) :
		errorFlag(false)
	{
		new (&data) T(std::move(value));
	}
	Result(Result<T, E>&& other) :
		errorFlag(other.errorFlag)
	{
		if (errorFlag) {
			new (&data) E(std::move(*(reinterpret_cast<E*>(&other.data))));
		} else {
			new (&data) T(std::move(*(reinterpret_cast<T*>(&other.data))));
		}
	}
	~Result() {
		if (errorFlag) {
			reinterpret_cast<E*>(&data)->~E();
		} else {
			reinterpret_cast<T*>(&data)->~T();
		}
	}

	operator bool() const {
		return isOk();
	}

	bool isOk() const {
		return !errorFlag;
	}
	bool isError() const {
		return errorFlag;
	}
	const T& ok() const {
		return *reinterpret_cast<T*>(&data);
	}
	const E& error() const {
		return *reinterpret_cast<E*>(&data);
	}
	T& ok() {
		return *reinterpret_cast<T*>(&data);
	}
	E& error() {
		return *reinterpret_cast<E*>(&data);
	}

	template <typename A, typename B>
	friend Result<A, B> result_ok(const A& value);

	template <typename A, typename B>
	friend Result<A, B> result_ok(A&& value);

	template <typename A, typename B>
	friend Result<A, B> result_err(const B& value);

	template <typename A, typename B>
	friend Result<A, B> result_err(B&& value);
};

template <typename T, typename E>
static Result<T, E> result_ok(const T& value) {
	Result<T, E> result;
	new (&result.data) T(value);
	return result;
}

template <typename T, typename E>
static Result<T, E> result_ok(T&& value) {
	Result<T, E> result;
	new (&result.data) T(std::move(value));
	return result;
}

template <typename T, typename E>
static Result<T, E> result_err(const E& value) {
	Result<T, E> result;
	new (&result.data) E(value);
	result.errorFlag = true;
	return result;
}

template <typename T, typename E>
static Result<T, E> result_err(E&& value) {
	Result<T, E> result;
	new (&result.data) E(std::move(value));
	result.errorFlag = true;
	return result;
}

}

#endif // RESULT_HPP
