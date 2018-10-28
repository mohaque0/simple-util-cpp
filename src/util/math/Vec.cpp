#include "util/math/Vec.hpp"

namespace Util {
namespace Math {

const Vec Vec::ORIGIN = Vec(0,0,0);

const Vec operator-(const Vec &v)
{
	return Vec(-v.x(), -v.y(), -v.z());
}

const Vec operator+(const Vec& p1, const Vec& p2) {
	return Vec(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
}

const Vec operator-(const Vec& p1, const Vec& p2) {
	return Vec(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z());
}

const Vec operator*(const double d, const Vec& v) {
	return Vec(v.x() * d, v.y() * d, v.z() * d);
}

const Vec operator*(const Vec& v, const double d) {
	return d * v;
}

const Vec operator/(const Vec &v, const double d)
{
	return Vec(v.x() / d, v.y() / d, v.z() / d);
}

const Point operator+(const Point& p, const Vec& v) {
	return Point(p.x() + v.x(), p.y() + v.y(), p.z() + v.z());
}

const Point operator-(const Point& p, const Vec& v) {
	return Point(p.x() - v.x(), p.y() - v.y(), p.z() - v.z());
}

bool Vec::operator==(const Vec &other) const
{
	return _x == other._x && _y == other._y && _z == other._z;
}

}
}
