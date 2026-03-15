/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SYSTEMTYPES_HPP_
#define SYSTEMTYPES_HPP_
#include <string>
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"

/// <summary>
/// Shorter, more clearer system types
/// </summary>
namespace Tudo
{
	typedef std::string				strg;
	typedef std::string_view		strgv;
	typedef signed char				int8;
	typedef short					int16;
	typedef long long				int64;
	typedef unsigned int			uint;
	typedef unsigned char			uint8;
	typedef unsigned short			uint16;
	typedef unsigned long long		uint64;
	typedef unsigned long			ulong;
	typedef Vector2<int>			vec2i;
	typedef Vector2<float>			vec2;
	typedef Vector2<double>			vec2d;
	typedef Vector3<int>			vec3i;
	typedef Vector3<float>			vec3;
	typedef Vector3<double>			vec3d;
	typedef Vector4<int>			vec4i;
	typedef Vector4<float>			vec4;
	typedef Vector4<double>			vec4d;
	typedef Matrix4<int>			mat4i;
	typedef Matrix4<float>			mat4;
	typedef Matrix4<double>			mat4d;
	typedef Quaternion<float>		quat;
}
#endif