#ifndef ROTATION_HPP
#define ROTATION_HPP

#include "util/math/Quaternion.hpp"
#include "util/math/Vec.hpp"

namespace Util {
namespace Math {

class Rot
{
	friend const Rot operator*(const Rot& r1, const Rot& r2);

private:
	// A unit quaternion representing the rotation.
	// TODO: INVARIANT: This must always be guaranteed to be the unit quaternion.
	Quat q;

public:	
	/**
	 * @brief Rot Set the rotation to be by this quaternion. THIS DOES NOT
	 * VERIFY THE QUATERNION'S MAGNITUDE = 1.
	 *
	 * @param q_
	 */
	explicit Rot(const Quat q_);

	/**
	 * @brief Rot Construct an object that represents a rotation by the given
	 * radians around the given axis.
	 *
	 * The direction of the rotation is clockwise if the direction is pointing
	 * away from the viewer. The magnitude of the direction vector is
	 * irrelevant.
	 *
	 * @param radians
	 * @param direction
	 * @return
	 */
	Rot(const double radians, const Vec& axis);
	~Rot();

	/**
	 * @brief getQuaternion Return this rotation as a unit quaternion.
	 * @return This rotation as a unit quaternion.
	 */
	const Quat getQuaternion() const;

	/**
	 * @brief getRadians Return the radians of the rotation.
	 *
	 * The value is positive if the rotation is clockwise when the axis
	 * points away from the viewer.
	 *
	 * @return The radians of the rotation.
	 */
	const double getRadians() const;

	/**
	 * @brief getDegrees Return the degrees of the rotation.
	 *
	 * The value is positive if the rotation is clockwise when the axis
	 * points away from the viewer.
	 *
	 * @return The degrees of the rotation.
	 */
	const double getDegrees() const;

	/**
	 * @brief getAxis Return the axis of rotation. The axis is not normalized.
	 * @return The axis of rotation. The axis is not normalized.
	 */
	const Vec getAxis() const;

	const Vec apply(const Vec& v) const;
};

const Rot operator*(const Rot& r1, const Rot& r2);
const double degreesToRadians(const double degrees);

}
}


#endif // ROTATION_HPP
