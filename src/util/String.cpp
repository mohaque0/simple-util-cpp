#include "String.hpp"

#include <cstddef>
#include <cstring>
#include <iostream>
#include <sstream>

namespace Util {

const size_t String::NOTFOUND = -1;

//
// StringData
//

StringData::StringData() :
	memory(),
	data(NULL),
	len(0)
{}

StringData::StringData(const char *str, bool copy) :
	memory(),
	data(str),
	len(strlen(str))
{
	if (copy) {
		char* buffer = new char[len];
		memcpy(buffer, str, len * sizeof(char));
		memory.reset(buffer, std::default_delete<char[]>());
		data = buffer;
	}
}

StringData::StringData(const char *str, size_t offset, size_t len_, bool copy) :
	memory(),
	data(&str[offset]),
	len(len_)
{
	if (copy) {
		char* buffer = new char[len];
		memcpy(buffer, &str[offset], len * sizeof(char));
		memory.reset(buffer, std::default_delete<char[]>());
		data = buffer;
	}
}

StringData::StringData(const StringData& other, size_t offset, size_t len_) :
	memory(other.memory),
	data(&other.data[offset]),
	len(len_)
{
	// Make sure we don't go past the end of memory.
	if (offset > other.size()) {
		offset = other.size();
		data = &other.data[offset];
	}
	if (offset + len > other.size()) {
		len = other.size() - offset;
	}
}

StringData::StringData(const std::vector<StringData> &sources) :
	memory(),
	data(NULL),
	len(0)
{
	// How big should we be?
	for (const StringData &datum : sources) {
		len += datum.size();
	}

	// Initialize storage.
	char* buffer = new char[len];

	// Copy data.
	char* ptr = buffer;
	for (const StringData &datum : sources) {
		memcpy(ptr, datum.get(), datum.size() * sizeof(char));
		ptr += datum.size();
	}
	memory.reset(buffer, std::default_delete<char[]>());
	data = buffer;
}

const char *StringData::get() const {
	return data;
}

size_t StringData::size() const
{
	return len;
}

//
// String
//

String::String(StringData datum)
{
	data.push_back(datum);
}

String::String()
{}

String::String(const char c)
{
	char str[2] = {c, '\0'};
	data.push_back(StringData(str));
}

String::String(const int i)
{
	std::stringstream stream;
	stream << i;
	data.push_back(StringData(stream.str().c_str()));
}

String::String(const float f)
{
	std::stringstream stream;
	stream << f;
	data.push_back(StringData(stream.str().c_str()));
}

String::String(const double d)
{
	std::stringstream stream;
	stream << d;
	data.push_back(StringData(stream.str().c_str()));
}

String::String(const long l)
{
	std::stringstream stream;
	stream << l;
	data.push_back(StringData(stream.str().c_str()));
}

String::String(const unsigned long l)
{
	std::stringstream stream;
	stream << l;
	data.push_back(StringData(stream.str().c_str()));
}

String::String(const char *str, bool copy)
{
	data.push_back(StringData(str, copy));
}

String::String(const char *str, size_t offset, size_t len, bool copy)
{
	data.push_back(StringData(str, offset, len, copy));
}

String::String(const std::string &other)
{
	data.push_back(StringData(other.c_str()));
}

String::String(const String &other) :
	data(other.data)
{}

String::String(String &&other) :
	data(std::move(other.data))
{}

void String::optimize() const
{
	if (data.size() > 1) {
		StringData newData(data);
		data.clear();
		data.push_back(newData);
	}
}

String &String::operator=(const String &other)
{
	data = other.data;
	return *this;
}

String &String::operator=(const char *str)
{
	StringData newData(str);
	data.clear();
	data.push_back(newData);
	return *this;
}

String &String::operator=(char c)
{
	char str[2] = {c, '\0'};
	StringData newData(str);
	data.clear();
	data.push_back(newData);
	return *this;
}

String String::operator+(const String &other) const
{
	String retVal;
	for (const StringData &datum : data) {
		retVal.data.push_back(datum);
	}
	for (const StringData &datum : other.data) {
		retVal.data.push_back(datum);
	}
	return retVal;
}

String &String::operator+=(const String &other)
{
	data.insert(data.end(), other.data.begin(), other.data.end());
	return *this;
}

String &String::operator+=(const char *str)
{
	*this += String(str);
	return *this;
}

String &String::operator+=(char c)
{
	// TODO: Very inefficient. Should realloc and append if possible?
	char str[2] = {c, '\0'};
	*this += String(str);
	return *this;
}

const char String::operator[](const size_t index) const
{
	return at(index);
}

const String String::substr(size_t start, size_t len) const
{
	optimize();

	if (data.size() == 0) {
		return String();
	}

	StringData newStringData(data[0], start, len);

	return String(newStringData);
}

const size_t String::size() const
{
	size_t s = 0;
	for (const auto& datum : data) {
		s += datum.size();
	}
	return s;
}

const char String::at(const size_t index) const
{
	// TODO: Perhaps there's a better way?
	optimize();
	return data[0].get()[index];
}

const size_t String::find(const String& toFind, const size_t start) const
{
	return find(toFind, start, toFind.size());
}

const size_t String::find(const String& toFind, const size_t start, const size_t matchLength) const
{
	optimize();

	for (size_t current = start; current < size() - matchLength; current++) {
		size_t idx = 0;
		for (idx = 0; idx < matchLength; idx++) {
			if (at(current + idx) != toFind.at(idx)) {
				break;
			}
		}

		// Found it!
		if (idx == matchLength) {
			return current;
		}
	}

	return NOTFOUND;
}

const std::shared_ptr<char> String::c_str() const
{
	char* buffer = new char[size() + 1];
	for (size_t i = 0; i < size(); i++) {
		buffer[i] = at(i);
	}
	buffer[size()] = '\0';
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}

std::ostream &operator<<(std::ostream &out, const String &str)
{
	for (const StringData &datum : str.data) {
		for (size_t i = 0; i < datum.size(); ++i) {
			const char * c = datum.get();
			out.put(c[i]);
		}
	}
	return out;
}

String operator+(const char* a, const String& b)
{
	return String(a) + b;
}

bool operator==(const String& a, const String& b)
{
	if (a.size() != b.size()) {
		return false;
	}

	for (size_t i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

bool operator!=(const String& a, const String& b)
{
	return !(a == b);
}

}
