#include "util/math/Point.hpp"

namespace Util {
namespace Math {

const Point Point::ORIGIN = Point(0,0,0);

bool Point::operator==(const Point &other) const
{
	return x() == other.x() && y() == other.y() && z() == other.z();
}

}
}
