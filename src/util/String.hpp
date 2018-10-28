#ifndef STRING_HPP
#define STRING_HPP

#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Util {

class String;
std::ostream &operator<<(std::ostream &out, const Util::String &str);

class StringData {
	// Hold a reference to memory if necessary.
	// This actually holds onto an array and uses a special deleter
	// because the template for shared_ptr is not specialized for T[]
	std::shared_ptr<char> memory;
	const char *data;
	size_t len;

public:
	StringData();
	StringData(const char *str, bool copy=true);
	StringData(const char *str, size_t offset, size_t len, bool copy=true);
	StringData(const StringData& other, size_t offset, size_t len);
	StringData(const std::vector<StringData> &sources);
	const char *get() const;
	size_t size() const;
};

class String
{
	friend std::ostream& Util::operator<<(std::ostream &out, const String &str);

	mutable std::vector<StringData> data;

	String(StringData datum);

public:
	String();
	String(const char c);
	String(const int i);
	String(const float f);
	String(const double d);
	String(const long l);
	String(const unsigned long lu);

	template<typename T>
	explicit String(const T t) {
		std::stringstream stream;
		stream << t;
		data.push_back(StringData(stream.str().c_str()));
	}

	String(const char *, bool copy=true);
	String(const char *, size_t offset, size_t len, bool copy=true);
	String(const std::string& other);
	String(const String& other);
	String(String&& other);

	/**
	 * @brief optimize Make read operations faster for this string.
	 */
	void optimize() const;

	String &operator=(const String &other);
	String &operator=(const char *str);
	String &operator=(char c);
	String operator+(const String &other) const;
	String &operator+=(const String &other);
	String &operator+=(const char *str);
	String &operator+=(char c);
	const char operator[](const size_t index) const;

	/**
	 * @brief substr Generate a substring based on a range. If the range
	 * is out of bounds then the range is cropped to be within bounds.
	 *
	 * @param start
	 * @param len
	 * @return A substring of this string with characters starting from
	 * start with length len.
	 */
	const String substr(size_t start, size_t len) const;

	/**
	 * @brief size
	 * @return The length of the string.
	 */
	const size_t size() const;

	const char at(const size_t index) const;

	/**
	 * @brief find Finds a search string in the original string. Will return
	 * the first index after start where the search string is found.
	 * @param toFind
	 * @param start
	 * @return Index at which the string was found or NOTFOUND otherwise.
	 */
	const size_t find(const String& toFind, const size_t start = 0) const;

	/**
	 * @brief find Finds a search string in the original string. Will return
	 * the first index after start where matchLength characters of the search
	 * string match.
	 *
	 * @param toFind
	 * @param start
	 * @param matchLength
	 * @return Index at which the string was found or NOTFOUND otherwise.
	 */
	const size_t find(const String& toFind, const size_t start, const size_t matchLength) const;

	/**
	 * @brief get Returns a pointer a null-terminated copy of the data in this String.
	 * @return A pointer a null-terminated copy of the data in this String.
	 */
	const std::shared_ptr<char> c_str() const;

	/**
	 * @brief NOTFOUND Constant value used to indicate a string was not found.
	 */
	static const size_t NOTFOUND;

};

String operator+(const char* a, const String& b);
bool operator==(const String& a, const String& b);
bool operator!=(const String& a, const String& b);

}

#endif // STRING_HPP
