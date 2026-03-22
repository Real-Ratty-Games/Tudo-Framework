/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_
#include <cmath>

namespace Tudo
{
	/// Vector 2D
	template<typename T>
	class Vector2
	{
	public:
		T X;
		T Y;

		Vector2() : X(T{}), Y(T{}) {}

		Vector2(T vl)
		{
			X = Y = vl;
		}

		Vector2(T x, T y)
		{
			X = x;
			Y = y;
		}

		Vector2(const Vector2& vec)
		{
			X = vec.X;
			Y = vec.Y;
		}

		template<typename U>
		Vector2(const Vector2<U>& other)
			: X(static_cast<T>(other.X)),
			Y(static_cast<T>(other.Y))
		{ }

		inline T* Ptr()
		{
			return &X;
		}

		inline const T* Ptr() const
		{
			return &X;
		}

		T Length() const
		{
			return std::sqrt(X * X + Y * Y);
		}

		Vector2<T> Normalized() const
		{
			const T len = Length();

			if (len == T(0))
				return Vector2<T>();

			return Vector2<T>(X / len, Y / len);
		}

		Vector2<T> Negate() const
		{
			return Vector2<T>(-X, -Y);
		}

		static inline T Dot(const Vector2<T>& left, const Vector2<T>& right)
		{
			return left.X * right.X + left.Y * right.Y;
		}

		Vector2<T> operator-() const
		{
			return Vector2<T>(-X, -Y);
		}

		bool operator == (const Vector2<T>& other) const
		{
			return (X == other.X && Y == other.Y);
		}

		Vector2<T> operator +(const Vector2<T>& other) const
		{
			return Vector2<T>(X + other.X, Y + other.Y);
		}

		Vector2<T> operator -(const Vector2<T>& other) const
		{
			return Vector2<T>(X - other.X, Y - other.Y);
		}

		Vector2<T> operator *(const Vector2<T>& other) const
		{
			return Vector2<T>(X * other.X, Y * other.Y);
		}

		Vector2<T> operator /(const Vector2<T>& other) const
		{
			return Vector2<T>(X / other.X, Y / other.Y);
		}

		Vector2<T>& operator +=(const Vector2<T>& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		Vector2<T>& operator -=(const Vector2<T>& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		Vector2<T>& operator *=(const Vector2<T>& other)
		{
			X *= other.X;
			Y *= other.Y;
			return *this;
		}

		Vector2<T>& operator /=(const Vector2<T>& other)
		{
			X /= other.X;
			Y /= other.Y;
			return *this;
		}
	};
}
#endif