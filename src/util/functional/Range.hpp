#ifndef RANGE_HPP
#define RANGE_HPP

#include <functional>
#include <vector>

namespace Util {
namespace Functional {

template <class InputType, class InputIterator, class OutputType, typename MapFunction>
class MappedRange;

template <class ValueType, class Iterator, typename FilterFunction>
class FilteredRange;


//
// Common methods for Ranges.
//

template<typename ValueType, typename InputIterator>
class Range {
public:
	Range() {}
	virtual ~Range() {}

	virtual InputIterator begin() = 0;
	virtual InputIterator end() = 0;

	template<typename FilterFunction>
	FilteredRange<ValueType, InputIterator, FilterFunction> filter(FilterFunction fn) {
		return FilteredRange<ValueType, InputIterator, FilterFunction>(*this, fn);
	}

	template<typename MapFunction>
	auto map(MapFunction fn) -> MappedRange<ValueType, InputIterator, decltype(fn(*begin())), MapFunction> {
		return MappedRange<ValueType, InputIterator, decltype(fn(*begin())), MapFunction>(*this, fn);
	}

	template <typename AggregatorFunction, typename AggregatorType>
	AggregatorType foldl(AggregatorFunction fn, AggregatorType initial) {
		AggregatorType v = initial;
		for (InputIterator i = begin(); i != end(); ++i) {
			v = fn(v, *i);
		}
		return v;
	}
};


//
// Basic Range: A range of all values between the given iterators [start,end).
//

template <typename ValueType, typename InputIterator>
class BasicRange : public Range<ValueType, InputIterator>
{
private:
	InputIterator startIter;
	InputIterator endIter;

public:
	BasicRange(InputIterator start, InputIterator end) :
		startIter(start),
		endIter(end)
	{}

	virtual ~BasicRange() {}

	virtual InputIterator begin() {
		return startIter;
	}

	virtual InputIterator end() {
		return endIter;
	}
};


//
// Filtered Range
//

template <class ValueType, class Iterator>
class FilteredRangeIterator {
private:
	Iterator startIter;
	Iterator endIter;
	std::function<bool(ValueType)> &fn;
public:
	FilteredRangeIterator(Iterator startIter_, Iterator endIter_, std::function<bool(ValueType)> &fn_) :
		startIter(startIter_),
		endIter(endIter_),
		fn(fn_)
	{
		while (startIter != endIter && !fn(*startIter)) {
			++startIter;
		}
	}
	~FilteredRangeIterator() {}

	bool operator==(const FilteredRangeIterator<ValueType, Iterator> &other) const {
		return startIter == other.startIter;
	}

	bool operator!=(const FilteredRangeIterator<ValueType, Iterator> &other) const {
		return !(startIter == other.startIter);
	}

	ValueType operator*() {
		return *startIter;
	}

	FilteredRangeIterator<ValueType, Iterator> operator++() {
		return FilteredRangeIterator<ValueType, Iterator>(++startIter, endIter, fn);
	}
};


template <class ValueType, class Iterator, typename FilterFunction>
class FilteredRange : public Range<ValueType, FilteredRangeIterator<ValueType, Iterator>> {
private:
	Range<ValueType, Iterator> &source;
	std::function<bool(ValueType)> fn;

public:
	FilteredRange(Range<ValueType, Iterator> &source_, FilterFunction &fn_) :
		source(source_),
		fn(fn_)
	{}

	virtual ~FilteredRange() {}

	virtual FilteredRangeIterator<ValueType, Iterator> begin() {
		return FilteredRangeIterator<ValueType, Iterator>(source.begin(), source.end(), fn);
	}

	virtual FilteredRangeIterator<ValueType, Iterator> end() {
		return FilteredRangeIterator<ValueType, Iterator>(source.end(), source.end(), fn);
	}
};


//
// Mapped Range
//

template <class InputType, class InputIterator, class OutputType>
class MappedRangeIterator {
private:
	InputIterator i;
	std::function<OutputType(InputType)> &fn;
public:
	MappedRangeIterator(InputIterator i_, std::function<OutputType(InputType)> &fn_) :
		i(i_),
		fn(fn_)
	{}
	~MappedRangeIterator() {}

	bool operator==(const MappedRangeIterator<InputType, InputIterator, OutputType> &other) const {
		return i == other.i;
	}

	bool operator!=(const MappedRangeIterator<InputType, InputIterator, OutputType> &other) const {
		return !(i == other.i);
	}

	OutputType operator*() {
		return fn(*i);
	}

	MappedRangeIterator<InputType, InputIterator, OutputType> operator++() {
		return MappedRangeIterator<InputType, InputIterator, OutputType>(++i, fn);
	}
};

template <class InputType, class InputIterator, class OutputType, typename MapFunction>
class MappedRange : public Range<OutputType, MappedRangeIterator<InputType, InputIterator, OutputType>>
{
private:
	Range<InputType, InputIterator> &source;
	std::function<OutputType(InputType)> fn;

public:
	MappedRange(Range<InputType, InputIterator> &source_, MapFunction& fn_) :
		source(source_),
		fn(fn_)
	{}

	virtual ~MappedRange() {}

	virtual MappedRangeIterator<InputType, InputIterator, OutputType> begin() {
		return MappedRangeIterator<InputType, InputIterator, OutputType>(source.begin(), fn);
	}

	virtual MappedRangeIterator<InputType, InputIterator, OutputType> end() {
		return MappedRangeIterator<InputType, InputIterator, OutputType>(source.end(), fn);
	}
};


//
// Static methods for easy construction without the need to supply template arguments.
//

template <class Iterable>
auto iterate(Iterable& i) -> BasicRange<decltype(*i.begin()), decltype(i.begin())> {
	return BasicRange<decltype(*i.begin()), decltype(i.begin())>(i.begin(), i.end());
}

template <class Iterator>
auto iterate(Iterator&& begin, Iterator&& end) -> BasicRange<decltype(*begin), Iterator> {
	return BasicRange<decltype(*begin), Iterator>(begin, end);
}


}
}


#endif // RANGE2_HPP
