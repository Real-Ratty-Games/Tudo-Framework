/*======================================================
    Copyright (c) 2026 Real Ratty Games.
    Created by Norbert Gerberg.
======================================================*/
#ifndef VECTOR4_HPP_
#define VECTOR4_HPP_
#include <cmath>

namespace Tudo
{
	/// Vector 4D
	template<typename T>
	class Vector4
	{
	public:
		T X;
		T Y;
		T Z;
		T W;

        Vector4() : X(T{}), Y(T{}), Z(T{}), W(T{}) {}

		Vector4(T vl)
		{
			X = Y = Z = W = vl;
		}

		Vector4(T x, T y, T z, T w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		Vector4(const Vector4& vec)
		{
			X = vec.X;
			Y = vec.Y;
			Z = vec.Z;
			W = vec.W;
		}

        template<typename U>
        Vector4(const Vector4<U>& other)
            : X(static_cast<T>(other.X)),
            Y(static_cast<T>(other.Y)),
            Z(static_cast<T>(other.Z)),
            W(static_cast<T>(other.W))
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
            return std::sqrt(X * X + Y * Y + Z * Z + W * W);
        }

        Vector4<T> Normalized() const
        {
            const T len = Length();

            if (len == T(0))
                return Vector4<T>();

            return Vector4<T>(X / len, Y / len, Z / len, W / len);
        }

        Vector4<T> Negate() const
        {
            return Vector4<T>(-X, -Y, -Z, -W);
        }

        static inline T Dot(const Vector4<T>& left, const Vector4<T>& right)
        {
            return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
        }

        Vector4<T> operator-() const
        {
            return Vector4<T>(-X, -Y, -Z, -W);
        }

        bool operator == (const Vector4<T>& other) const
        {
            return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
        }

        Vector4<T> operator +(const Vector4<T>& other) const
        {
            return Vector4<T>(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
        }

        Vector4<T> operator -(const Vector4<T>& other) const
        {
            return Vector4<T>(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
        }

        Vector4<T> operator *(const Vector4<T>& other) const
        {
            return Vector4<T>(X * other.X, Y * other.Y, Z * other.Z, W * other.W);
        }

        Vector4<T> operator /(const Vector4<T>& other) const
        {
            return Vector4<T>(X / other.X, Y / other.Y, Z / other.Z, W / other.W);
        }

        Vector4<T>& operator +=(const Vector4<T>& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            W += other.W;
            return *this;
        }

        Vector4<T>& operator -=(const Vector4<T>& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
            W -= other.W;
            return *this;
        }

        Vector4<T>& operator *=(const Vector4<T>& other)
        {
            X *= other.X;
            Y *= other.Y;
            Z *= other.Z;
            W *= other.W;
            return *this;
        }

        Vector4<T>& operator /=(const Vector4<T>& other)
        {
            X /= other.X;
            Y /= other.Y;
            Z /= other.Z;
            W /= other.W;
            return *this;
        }
    };
}
#endif