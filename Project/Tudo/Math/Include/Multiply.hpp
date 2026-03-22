/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MULTIPLY_HPP_
#define MULTIPLY_HPP_
#include "SystemTypes.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"

namespace Tudo::Math
{
	template<typename T>
	Vector4<T> Multiply(const Matrix4<T>& left, const Vector4<T>& right)
	{
		return Vector4<T>(
			left(0, 0) * right.X + left(0, 1) * right.Y + left(0, 2) * right.Z + left(0, 3) * right.W,
			left(1, 0) * right.X + left(1, 1) * right.Y + left(1, 2) * right.Z + left(1, 3) * right.W,
			left(2, 0) * right.X + left(2, 1) * right.Y + left(2, 2) * right.Z + left(2, 3) * right.W,
			left(3, 0) * right.X + left(3, 1) * right.Y + left(3, 2) * right.Z + left(3, 3) * right.W
		);
	}

	template<typename T>
	Vector4<T> Multiply(const Vector4<T>& left, const Matrix4<T>& right)
	{
		return Vector4<T>(
			left.X * right(0, 0) + left.Y * right(1, 0) + left.Z * right(2, 0) + left.W * right(3, 0),
			left.X * right(0, 1) + left.Y * right(1, 1) + left.Z * right(2, 1) + left.W * right(3, 1),
			left.X * right(0, 2) + left.Y * right(1, 2) + left.Z * right(2, 2) + left.W * right(3, 2),
			left.X * right(0, 3) + left.Y * right(1, 3) + left.Z * right(2, 3) + left.W * right(3, 3)
		);
	}

	template<typename T>
	Vector3<T> Multiply(const Vector3<T>& left, const Quaternion<T>& right)
	{
		Vector3<T> u = Vector3<T>(right.X, right.Y, right.Z);
		T s = right.W;

		return u * 2 * Vector3<T>::Dot(u, left)
			+ left * (s * s - Vector3<T>::Dot(u, u))
			+ Vector3<T>::Cross(u, left) * 2 * s;
	}
}
#endif