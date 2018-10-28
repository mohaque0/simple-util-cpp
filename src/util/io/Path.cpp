#include "Path.hpp"

#include "system/Constants.hpp"

namespace Util {
namespace IO {

void Path::init(const String& prefix_, const String& separator_, const String& path)
{
	absolute = false;
	prefix = prefix_;
	separator = separator_;

	size_t i = 0;

	if (path.size() > 0 && path.substr(0, separator.size()) == separator) {
		absolute = true;
		i = separator.size();
	}
	if (prefix != "") {
		absolute = true;
	}

	while (i < path.size()) {
		size_t loc = path.find(separator, i);
		if (loc == String::NOTFOUND) {
			names.push_back(path.substr(i, path.size()-i));
			break;
		}

		names.push_back(path.substr(i, loc-i));
		i = loc + separator.size();
	}
}

Path::Path(const String& path)
{
	init("", System::fileSeparator(), path);
}

Path::Path(const String& separator_, const String& path)
{
	init("", System::fileSeparator(), path);
}

bool Path::isAbsolute() const
{
	return absolute;
}

const String Path::getPrefix() const
{
	return prefix;
}

const std::vector<String> Path::getNames() const
{
	return names;
}

const String Path::getSeparator() const
{
	return separator;
}

const String Path::getString() const
{
	String retVal;
	retVal += getPrefix();
	int first = true;

	for (const String& name : getNames()) {
		retVal += (first && !isAbsolute() ? "" : getSeparator()) + name;
		first = false;
	}
	return retVal;
}

std::ostream& operator<<(std::ostream& out, const Path& path)
{
	out << path.getString();
	return out;
}

}
}
