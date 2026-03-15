/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Transformation.hpp"

using namespace Tudo;

quat Math::QuatFromAxisAngle(const vec3& axis, float angle)
{
	const vec3 naxis = axis.Normalized();
	const float half = angle * 0.5f;
	const float s = std::sin(half);
	return quat(
		std::cos(half),
		naxis.X * s,
		naxis.Y * s,
		naxis.Z * s
	);
}

quat Math::QuatFromEulerAngles(const EulerRotation& rot)
{
	const float halfYaw		= rot.Yaw * 0.5f;
	const float halfPitch	= rot.Pitch * 0.5f;
	const float halfRoll	= rot.Roll * 0.5f;

	const float cy = std::cos(halfYaw);
	const float sy = std::sin(halfYaw);
	const float cp = std::cos(halfPitch);
	const float sp = std::sin(halfPitch);
	const float cr = std::cos(halfRoll);
	const float sr = std::sin(halfRoll);

	return quat(
		cr * cp * cy + sr * sp * sy,
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy 
	);
}

quat Math::QuatAddEulerRotation(const quat& q, const EulerRotation& rot, bool local)
{
	const quat q2 = QuatFromEulerAngles(rot);
	quat result =  local ? (q * q2) : (q2 * q);
	result = result.Normalized();
	return result;
}
