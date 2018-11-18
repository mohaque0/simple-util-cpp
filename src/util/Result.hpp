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
	enum class State {
		Ok,
		Error,
		Invalid // This is only used for moved or uninitialized objects.
	};

	// This guarantees we have aligned space large enough for both types.
	struct data_storage {
		alignas(std::max({alignof(T), alignof(E)})) uint8_t bytes[std::max({sizeof(T), sizeof(E)})];
	} data;
	State stateFlag;

	/**
	 * @brief Result
	 *
	 * This constructor should only be used by the static methods result_ok and result_err.
	 * Ideally we want result objects to be valid as much as possible.
	 */
	Result() :
		stateFlag(State::Invalid)
	{}

	/**
	 * @brief moveData
	 *
	 * Precondition: stateFlag is set correctly for the data that is to be moved into this result.
	 *
	 * @param data
	 */
	void moveData(void* otherData) {
		if (stateFlag == State::Error) {
			new (&data.bytes) E(std::move(*(reinterpret_cast<E*>(otherData))));
		} else if (stateFlag == State::Ok) {
			new (&data.bytes) T(std::move(*(reinterpret_cast<T*>(otherData))));
		}
	}

	void destroyData() {
		if (stateFlag == State::Error) {
			reinterpret_cast<E*>(&data.bytes)->~E();
		} else if (stateFlag == State::Ok) {
			reinterpret_cast<T*>(&data.bytes)->~T();
		}
	}

public:
	Result(T&& value) :
		stateFlag(false)
	{
		new (&data.bytes) T(std::move(value));
	}
	Result(Result<T, E>&& other) :
		stateFlag(other.stateFlag)
	{
		moveData(&other.data.bytes);

		// Prevent destructors on invalid data.
		other.stateFlag = State::Invalid;
	}
	~Result() {
		destroyData();
	}
	Result& operator=(Result&& other) {
		if (this == &other) {
			return *this;
		}

		destroyData();
		this->stateFlag = other.stateFlag;
		moveData(&other.data.bytes);

		// Prevent destructors on invalid data.
		other.stateFlag = State::Invalid;

		return *this;
	}

	operator bool() const {
		return isOk();
	}

	bool isOk() const {
		return stateFlag == State::Ok;
	}
	bool isError() const {
		return stateFlag == State::Error;
	}
	__attribute__ ((noinline)) const T& ok() const {
		return *reinterpret_cast<T*>(&data.bytes);
	}
	__attribute__ ((noinline)) const E& error() const {
		return *reinterpret_cast<E*>(&data.bytes);
	}
	__attribute__ ((noinline)) T& ok() {
		return *reinterpret_cast<T*>(&data.bytes);
	}
	__attribute__ ((noinline)) E& error() {
		return *reinterpret_cast<E*>(&data.bytes);
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
	new (&result.data.bytes) T(value);
	result.stateFlag = Result<T,E>::State::Ok;
	return result;
}

template <typename T, typename E>
static Result<T, E> result_ok(T&& value) {
	Result<T, E> result;
	new (&result.data.bytes) T(std::move(value));
	result.stateFlag = Result<T,E>::State::Ok;
	return result;
}

template <typename T, typename E>
static Result<T, E> result_err(const E& value) {
	Result<T, E> result;
	new (&result.data.bytes) E(value);
	result.stateFlag = Result<T,E>::State::Error;
	return result;
}

template <typename T, typename E>
static Result<T, E> result_err(E&& value) {
	Result<T, E> result;
	new (&result.data.bytes) E(std::move(value));
	result.stateFlag = Result<T,E>::State::Error;
	return result;
}

}

#endif // RESULT_HPP
