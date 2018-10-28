#ifndef PATH_HPP
#define PATH_HPP

#include "util/String.hpp"

#include <ostream>
#include <vector>

namespace Util {
namespace IO {

class Path
{
	bool absolute;
	String prefix;
	String separator;
	std::vector<String> names;

	void init(const String& prefix, const String& separator, const String& path);

public:
	Path(const String& path);
	Path(const String& separator, const String& path);

	bool isAbsolute() const;
	const String getPrefix() const;
	const std::vector<String> getNames() const;
	const String getSeparator() const;
	const String getString() const;
};

std::ostream& operator<<(std::ostream &out, const Path &path);

}
}


#endif // PATH_HPP
