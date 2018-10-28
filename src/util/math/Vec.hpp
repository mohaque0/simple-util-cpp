#pragma once

#include "util/math/Point.hpp"
#include "util/logging/Logging.hpp"
#include <math.h>

namespace Util {
namespace Math {

class Vec {
private:
	double _x;
	double _y;
	double _z;

public:
	Vec(const double x, const double y, const double z) :
		_x(x),
		_y(y),
		_z(z)
	{}

	Vec(const Point& origin, const Point& destination) :
		_x(destination.x() - origin.x()),
		_y(destination.y() - origin.y()),
		_z(destination.z() - origin.z())
	{}

	const double x() const { return _x; }
	const double y() const { return _y; }
	const double z() const { return _z; }

	const double magnitude() const {
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

	bool operator==(const Vec& other) const;

	static Vec normalize(const Vec& v) {
		const double m = v.magnitude();
		if (m > 0) {
			return Vec(v.x() / m, v.y() / m, v.z() / m);
		} else {
			return v;
		}
	}

	static Vec cross(const Vec& v0, const Vec& v1) {
		double x = v0.y() * v1.z() - v0.z() * v1.y();
		double y = -(v0.x() * v1.z() - v0.z() * v1.x());
		double z = v0.x() * v1.y() - v0.y() * v1.x();
		return Vec(x,y,z);
	}

	static double dot(const Vec& v0, const Vec& v1) {
		return v0.x()*v1.x() + v0.y()+v1.y() + v0.z()+v1.z();
	}

	static const Vec ORIGIN;
};

const Vec operator-(const Vec& v);
const Vec operator+(const Vec& p1, const Vec& p2);
const Vec operator-(const Vec& p1, const Vec& p2);
const Vec operator*(const double d, const Vec& v);
const Vec operator*(const Vec& v, const double d);
const Vec operator/(const Vec& v, const double d);
const Point operator+(const Point& p, const Vec& v);
const Point operator-(const Point& p, const Vec& v);

}
}

namespace Util {
namespace Logging {

template<>
inline String logString(const Util::Math::Vec &v) {
	return String() + '<' + v.x() + "," + v.y() + "," + v.z() + '>';
}

}
}
