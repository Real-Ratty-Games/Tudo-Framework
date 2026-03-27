/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MATRIX4_HPP_
#define MATRIX4_HPP_
#include "Math.hpp"

namespace Tudo
{
	/// Matrix 4x4
	/// row-major style
	template<typename T>
	class Matrix4
	{
	public:
		T Data[16];

		static Matrix4<T> Identity()
		{
			return Matrix4<T>(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}

		Matrix4<T>() : Data{} {}

		Matrix4<T>(T vl)
			: Data {
				vl, vl, vl, vl,
				vl, vl, vl, vl,
				vl, vl, vl, vl,
				vl, vl, vl, vl
		} { }

		Matrix4<T>(T a1, T a2, T a3, T a4,
			T b1, T b2, T b3, T b4, 
			T c1, T c2, T c3, T c4, 
			T d1, T d2, T d3, T d4)
			: Data{
			a1, a2, a3, a4,
			b1, b2, b3, b4,
			c1, c2, c3, c4,
			d1, d2, d3, d4
		} { }

		Matrix4<T>(const T (&data)[16])
		{
			for (int i = 0; i < 16; i++)
				Data[i] = data[i];
		}

		inline T* Ptr()
		{
			return &Data[0];
		}

		inline const T* Ptr() const
		{
			return &Data[0];
		}

		bool Inverse(Matrix4<T>& outMat)
		{
			const T* m = Data;
			T inv[16], det;

			inv[0] = m[5] * m[10] * m[15] -
				m[5] * m[11] * m[14] -
				m[9] * m[6] * m[15] +
				m[9] * m[7] * m[14] +
				m[13] * m[6] * m[11] -
				m[13] * m[7] * m[10];

			inv[4] = -m[4] * m[10] * m[15] +
				m[4] * m[11] * m[14] +
				m[8] * m[6] * m[15] -
				m[8] * m[7] * m[14] -
				m[12] * m[6] * m[11] +
				m[12] * m[7] * m[10];

			inv[8] = m[4] * m[9] * m[15] -
				m[4] * m[11] * m[13] -
				m[8] * m[5] * m[15] +
				m[8] * m[7] * m[13] +
				m[12] * m[5] * m[11] -
				m[12] * m[7] * m[9];

			inv[12] = -m[4] * m[9] * m[14] +
				m[4] * m[10] * m[13] +
				m[8] * m[5] * m[14] -
				m[8] * m[6] * m[13] -
				m[12] * m[5] * m[10] +
				m[12] * m[6] * m[9];

			inv[1] = -m[1] * m[10] * m[15] +
				m[1] * m[11] * m[14] +
				m[9] * m[2] * m[15] -
				m[9] * m[3] * m[14] -
				m[13] * m[2] * m[11] +
				m[13] * m[3] * m[10];

			inv[5] = m[0] * m[10] * m[15] -
				m[0] * m[11] * m[14] -
				m[8] * m[2] * m[15] +
				m[8] * m[3] * m[14] +
				m[12] * m[2] * m[11] -
				m[12] * m[3] * m[10];

			inv[9] = -m[0] * m[9] * m[15] +
				m[0] * m[11] * m[13] +
				m[8] * m[1] * m[15] -
				m[8] * m[3] * m[13] -
				m[12] * m[1] * m[11] +
				m[12] * m[3] * m[9];

			inv[13] = m[0] * m[9] * m[14] -
				m[0] * m[10] * m[13] -
				m[8] * m[1] * m[14] +
				m[8] * m[2] * m[13] +
				m[12] * m[1] * m[10] -
				m[12] * m[2] * m[9];

			inv[2] = m[1] * m[6] * m[15] -
				m[1] * m[7] * m[14] -
				m[5] * m[2] * m[15] +
				m[5] * m[3] * m[14] +
				m[13] * m[2] * m[7] -
				m[13] * m[3] * m[6];

			inv[6] = -m[0] * m[6] * m[15] +
				m[0] * m[7] * m[14] +
				m[4] * m[2] * m[15] -
				m[4] * m[3] * m[14] -
				m[12] * m[2] * m[7] +
				m[12] * m[3] * m[6];

			inv[10] = m[0] * m[5] * m[15] -
				m[0] * m[7] * m[13] -
				m[4] * m[1] * m[15] +
				m[4] * m[3] * m[13] +
				m[12] * m[1] * m[7] -
				m[12] * m[3] * m[5];

			inv[14] = -m[0] * m[5] * m[14] +
				m[0] * m[6] * m[13] +
				m[4] * m[1] * m[14] -
				m[4] * m[2] * m[13] -
				m[12] * m[1] * m[6] +
				m[12] * m[2] * m[5];

			inv[3] = -m[1] * m[6] * m[11] +
				m[1] * m[7] * m[10] +
				m[5] * m[2] * m[11] -
				m[5] * m[3] * m[10] -
				m[9] * m[2] * m[7] +
				m[9] * m[3] * m[6];

			inv[7] = m[0] * m[6] * m[11] -
				m[0] * m[7] * m[10] -
				m[4] * m[2] * m[11] +
				m[4] * m[3] * m[10] +
				m[8] * m[2] * m[7] -
				m[8] * m[3] * m[6];

			inv[11] = -m[0] * m[5] * m[11] +
				m[0] * m[7] * m[9] +
				m[4] * m[1] * m[11] -
				m[4] * m[3] * m[9] -
				m[8] * m[1] * m[7] +
				m[8] * m[3] * m[5];

			inv[15] = m[0] * m[5] * m[10] -
				m[0] * m[6] * m[9] -
				m[4] * m[1] * m[10] +
				m[4] * m[2] * m[9] +
				m[8] * m[1] * m[6] -
				m[8] * m[2] * m[5];

			det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (std::abs(det) < T(1e-6))
				return false;

			det = 1 / det;

			for (int i = 0; i < 16; i++)
				outMat.Data[i] = inv[i] * det;

			return true;
		}

		T& operator()(int row, int col) { return Data[row * 4 + col]; }
		const T& operator()(int row, int col) const { return Data[row * 4 + col]; }

		bool operator == (const Matrix4<T>& other) const
		{
			for (size_t i = 0; i < 16; i++)
			{
				if (Data[i] != other.Data[i])
					return false;
			}
			return true;
		}

		Matrix4<T> operator +(const Matrix4<T>& other) const
		{
			Matrix4<T> result(T(0));
			for (size_t i = 0; i < 16; i++)
				result.Data[i] = Data[i] + other.Data[i];
			return result;
		}

		Matrix4<T> operator -(const Matrix4<T>& other) const
		{
			Matrix4<T> result(T(0));
			for (size_t i = 0; i < 16; i++)
				result.Data[i] = Data[i] - other.Data[i];
			return result;
		}

		Matrix4<T> operator *(const Matrix4<T>& other) const
		{
			Matrix4<T> result(T(0));
			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int k = 0; k < 4; k++)
						result(row, col) += Data[row * 4 + k] * other(k, col);
			return result;
		}

		Matrix4<T>& operator +=(const Matrix4<T>& other)
		{
			for (size_t i = 0; i < 16; i++)
				Data[i] += other.Data[i];
			return *this;
		}

		Matrix4<T>& operator -=(const Matrix4<T>& other)
		{
			for (size_t i = 0; i < 16; i++)
				Data[i] -= other.Data[i];
			return *this;
		}

		Matrix4<T>& operator *=(const Matrix4<T>& other)
		{
			Matrix4<T> result(T(0));
			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int k = 0; k < 4; k++)
						result(row, col) += Data[row * 4 + k] * other(k, col);
			*this = result;
			return *this;
		}
	};
}
#endif
