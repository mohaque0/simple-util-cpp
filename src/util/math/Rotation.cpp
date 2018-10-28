#include "Rotation.hpp"

#include "util/math/Constants.hpp"

namespace Util {
namespace Math {

Rot::Rot(const Quat q_) :
	q(q_)
{}

Rot::Rot(const double radians, const Vec &axis) :
	q(Quat::versor(Quat(cos(radians / 2), (sin(radians / 2) * axis))))
{}

Rot::~Rot() {}

const Quat Rot::getQuaternion() const
{
	return q;
}

const double Rot::getRadians() const
{
	return acos(q.w()) * 2;
}

const double Rot::getDegrees() const
{
	return getRadians() * DEGREES_PER_RADIAN;
}

const Vec Rot::getAxis() const
{
	return Vec(q.x(), q.y(), q.z());
}

const Vec Rot::apply(const Vec &v) const
{
	// Because we are guaranteed that q is a unit quaternion, we can just
	// use the conjugate of q rather than the inverse.
	Quat result = q * Quat(0, v) * Quat::conjugate(q);
	return Vec(result.x(), result.y(), result.z());
}

const Rot operator*(const Rot &r1, const Rot &r2)
{
	// The magnitude is multiplicative.
	// We are quaranteed each quaternion has magnitude 1, so the result also
	// has magnitude 1.
	return Rot(r1.q * r2.q);
}

const double degreesToRadians(const double degrees)
{
	return degrees * RADIANS_PER_DEGREE;
}

}
}


