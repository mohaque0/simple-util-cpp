#ifndef OPTION_HPP
#define OPTION_HPP

#include <functional>
#include <memory>

namespace Util {

template<typename T>
class Option
{
private:
	std::unique_ptr<T> value;

public:
	Option() {}
	Option(T&& t) :
		value(std::make_unique<T>(t))
	{}
	~Option() {}

	T& get() {
		return *value;
	}

	const T& get() const {
		return *value;
	}

	bool isSome() const {
		return value.get() != NULL;
	}

	template <typename F>
	const Option<T>& isSome(F f) const {
		if (isSome()) {
			f(*value.get());
		}
		return *this;
	}

	bool isNone() const {
		return !isSome();
	}

	template <typename F>
	const Option<T>& isNone(F f) const {
		if (isNone()) {
			f();
		}
		return *this;
	}
};

template <typename T>
static Option<T> make_some(T&& t) {
	return Option<T>(t);
}

template <typename T>
static Option<T> make_none() {
	return Option<T>();
}

}

#endif // OPTION_HPP
