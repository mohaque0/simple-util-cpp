#include "Quaternion.hpp"

#include <cstdio>
#include <math.h>

namespace Util {
namespace Math {

Quat::Quat(const double real, const Vec& imaginary) :
	w_(real),
	x_(imaginary.x()),
	y_(imaginary.y()),
	z_(imaginary.z())
{}

Quat::Quat(const double w__, const double x__, const double y__, const double z__) :
	w_(w__),
	x_(x__),
	y_(y__),
	z_(z__)
{}

Quat::~Quat() {}

const double Quat::w() const
{
	return w_;
}

const double Quat::x() const
{
	return x_;
}

const double Quat::y() const
{
	return y_;
}

const double Quat::z() const
{
	return z_;
}

const double Quat::norm() const
{
	return sqrt(w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_);
}

Quat Quat::conjugate(const Quat& q)
{
	return Quat(q.w(), -q.x(), -q.y(), -q.z());
}

Quat Quat::inverse(const Quat &q)
{
	return conjugate(q) / q.norm();
}

Quat Quat::versor(const Quat &q)
{
	return q / q.norm();
}

const Quat operator-(const Quat &q)
{
	return Quat(-q.w(), -q.x(), -q.y(), -q.z());
}

const Quat operator+(const Quat &q1, const Quat &q2)
{
	return Quat(q1.w() + q2.w(), q1.x() + q2.x(), q1.y() + q2.y(), q1.z() + q2.z());
}

const Quat operator-(const Quat &q1, const Quat &q2)
{
	return q1 + (-q2);
}

const Quat operator*(const double d, const Quat &q)
{
	return Quat(q.w() * d, q.x() * d, q.y() * d, q.z() * d);
}

const Quat operator*(const Quat &q, const double d)
{
	return d * q;
}

const Quat operator/(const Quat &q, const double d)
{
	return q * (1 / d);
}

const Quat operator*(const Quat &a, const Quat &b)
{
	double w = a.w()*b.w() - a.x()*b.x() - a.y()*b.y() - a.z()*b.z();
	double x = a.w()*b.x() + a.x()*b.w() + a.y()*b.z() - a.z()*b.y();
	double y = a.w()*b.y() - a.x()*b.z() + a.y()*b.w() + a.z()*b.x();
	double z = a.w()*b.z() + a.x()*b.y() - a.y()*b.x() + a.z()*b.w();

	return Quat(w, x, y, z);
}

}
}

