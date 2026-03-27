/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef TRANSFORMATION_HPP_
#define TRANSFORMATION_HPP_
#include "EulerRotation.hpp"

namespace Tudo::Math
{
	template<typename T>
	Matrix4<T> Translate(const Matrix4<T>& mat, const Vector3<T>& v, bool local)
	{
		Matrix4<T> result = Matrix4<T>::Identity();
		result(3, 0) = v.X;
		result(3, 1) = v.Y;
		result(3, 2) = v.Z;
		return local ? (mat * result) : (result * mat);
	}

	template<typename T>
	Matrix4<T> Scale(const Matrix4<T>& mat, const Vector3<T>& v, bool local)
	{
		Matrix4<T> result = Matrix4<T>::Identity();
		result(0, 0) = v.X;
		result(1, 1) = v.Y;
		result(2, 2) = v.Z;
		return local ? (mat * result) : (result * mat);
	}

	/// angle in radians
	template<typename T>
	Matrix4<T> Rotate(const Matrix4<T>& mat, const Vector3<T>& axis, T angle, bool local)
	{
		const Vector3<T> u = axis.Normalized();
		const T c = std::cos(angle);
		const T s = std::sin(angle);
		const T t = 1 - c;

		Matrix4<T> result = Matrix4<T>::Identity();
		result(0, 0) = t * u.X * u.X + c;
		result(1, 0) = t * u.X * u.Y - s * u.Z;
		result(2, 0) = t * u.X * u.Z + s * u.Y;

		result(0, 1) = t * u.X * u.Y + s * u.Z;
		result(1, 1) = t * u.Y * u.Y + c;
		result(2, 1) = t * u.Y * u.Z - s * u.X;

		result(0, 2) = t * u.X * u.Z - s * u.Y;
		result(1, 2) = t * u.Y * u.Z + s * u.X;
		result(2, 2) = t * u.Z * u.Z + c;
		return local ? (mat * result) : (result * mat);
	}

	template<typename T>
	Matrix4<T> Rotate(const Matrix4<T>& mat, const Quaternion<T>& q, bool local)
	{
		Quaternion<T> nq = q.Normalized();
		T xx = nq.X * nq.X;
		T yy = nq.Y * nq.Y;
		T zz = nq.Z * nq.Z;
		T xy = nq.X * nq.Y;
		T xz = nq.X * nq.Z;
		T yz = nq.Y * nq.Z;
		T wx = nq.W * nq.X;
		T wy = nq.W * nq.Y;
		T wz = nq.W * nq.Z;

		Matrix4<T> result = Matrix4<T>::Identity();
		result(0, 0) = 1 - 2 * (yy + zz);
		result(0, 1) = 2 * (xy - wz);
		result(0, 2) = 2 * (xz + wy);

		result(1, 0) = 2 * (xy + wz);
		result(1, 1) = 1 - 2 * (xx + zz);
		result(1, 2) = 2 * (yz - wx);

		result(2, 0) = 2 * (xz - wy);
		result(2, 1) = 2 * (yz + wx);
		result(2, 2) = 1 - 2 * (xx + yy);
		
		Matrix4<T> out = local ? (mat * result) : (result * mat);
		out(3, 0) = mat(3, 0);
		out(3, 1) = mat(3, 1);
		out(3, 2) = mat(3, 2);
		out(3, 3) = mat(3, 3);
		return out;
	}

	template<typename T>
	Vector3<T> Rotate(const Quaternion<T>& q, const Vector3<T>& v)
	{
		Quaternion<T> p(0, v.X, v.Y, v.Z);
		Quaternion<T> qInv = q.Conjugate();
		Quaternion<T> result = q * p * qInv;
		return { result.X, result.Y, result.Z };
	}

	/// angle in radians
	quat QuatFromAxisAngle(const vec3& axis, float angle);
	quat QuatFromEulerAngles(const EulerRotation& rot);
	quat QuatAddEulerRotation(const quat& q, const EulerRotation& rot, bool local);
}
#endif