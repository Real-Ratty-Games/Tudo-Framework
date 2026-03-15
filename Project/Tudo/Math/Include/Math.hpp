/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MATH_HPP_
#define MATH_HPP_

#define TUDO_MATH_PI 3.14159265359

namespace Tudo::Math
{
	template<typename T>
	constexpr const T& Maxi(const T& a, const T& b) noexcept
	{
		return (b < a) ? a : b;
	}

	template<typename T>
	constexpr const T& Mini(const T& a, const T& b) noexcept
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	constexpr T ToRadians(const T& vl) noexcept
	{
		return (T(TUDO_MATH_PI) / T(180)) * vl;
	}

	template<typename T>
	constexpr T ToDegree(const T& vl) noexcept
	{
		return (T(180) / T(TUDO_MATH_PI)) * vl;
	}

	template<typename T>
	inline T Distance(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a - b).Length();
	}

	template<typename T>
	inline T Distance(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a - b).Length();
	}

	template<typename T>
	inline T Distance(const Vector4<T>& a, const Vector4<T>& b)
	{
		return (a - b).Length();
	}
}
#endif
