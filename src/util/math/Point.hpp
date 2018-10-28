#pragma once

#include "util/logging/Logging.hpp"

namespace Util {
namespace Math {

class Point {
private:
	double _x;
	double _y;
	double _z;

public:
	Point(const double x, const double y, const double z) :
		_x(x),
		_y(y),
		_z(z)
	{}

	const double x() const { return _x; }
	const double y() const { return _y; }
	const double z() const { return _z; }

	bool operator==(const Point& other) const;

	static const Point ORIGIN;
};

}
}


namespace Util {
namespace Logging {

template<>
inline String logString(const Util::Math::Point &p) {
	return String() + '(' + p.x() + "," + p.y() + "," + p.z() + ')';
}

}
}
