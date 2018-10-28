#ifndef QUAT_HPP
#define QUAT_HPP

#include "util/math/Vec.hpp"

namespace Util {
namespace Math {

class Quat
{
private:
	double w_;
	double x_;
	double y_;
	double z_;

public:

	/**
	 * @brief Quat Construct a quaternion with the given
	 * real component and the vector as the non-real components.
	 *
	 * @param real
	 * @param imaginary
	 */
	Quat(const double real, const Vec& imaginary);

	/**
	 * @brief Quat Construct a quaternion with the given values.
	 * @param w The real component of the quaternion.
	 * @param x The first imaginary component.
	 * @param y The second imaginary component.
	 * @param z The third imaginary component.
	 */
	Quat(const double w__, const double x__, const double y__, const double z__);
	~Quat();

	const double w() const;
	const double x() const;
	const double y() const;
	const double z() const;

	const double norm() const;

	static Quat conjugate(const Quat& q);
	static Quat inverse(const Quat& q);

	/**
	 * @brief versor Return the versor (unit quaternion).
	 * @param q
	 * @return The versor (unit quaternion).
	 */
	static Quat versor(const Quat& q);

};

const Quat operator-(const Quat& q);
const Quat operator+(const Quat& q1, const Quat& q2);
const Quat operator-(const Quat& q1, const Quat& q2);
const Quat operator*(const double d, const Quat& q);
const Quat operator*(const Quat& q, const double d);
const Quat operator/(const Quat& q, const double d);
const Quat operator*(const Quat& a, const Quat& b);

}
}


#endif // QUAT_HPP
