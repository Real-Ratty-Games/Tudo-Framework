/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_
#include "Vector3.hpp"

namespace Tudo
{
	template<typename T>
	class Quaternion
	{
	public:
		T W;
		T X;
		T Y;
		T Z;

		Quaternion() : W(T{}), X(T{}), Y(T{}), Z(T{}) {}

		Quaternion(T vl)
		{
			W = X = Y = Z = vl;
		}

		Quaternion(T w, T x, T y, T z)
		{
			W = w;
			X = x;
			Y = y;
			Z = z;
		}

		inline T* Ptr()
		{
			return &W;
		}

		inline const T* Ptr() const
		{
			return &W;
		}

		T Length() const
		{
			return std::sqrt(W * W + X * X + Y * Y + Z * Z);
		}

		Quaternion<T> Normalized() const
		{
			const T len = Length();

			if (len == 0)
				return Quaternion<T>();

			return Quaternion<T>(
				W / len,
				X / len,
				Y / len,
				Z / len
			);
		}

		Quaternion<T> Conjugate() const
		{
			return { W, -X, -Y, -Z };
		}

		Quaternion<T> operator*(const Quaternion<T>& q) const
		{
			return {
				W * q.W - X * q.X - Y * q.Y - Z * q.Z,
				W * q.X + X * q.W + Y * q.Z - Z * q.Y,
				W * q.Y - X * q.Z + Y * q.W + Z * q.X,
				W * q.Z + X * q.Y - Y * q.X + Z * q.W
			};
		}
	};
}
#endif