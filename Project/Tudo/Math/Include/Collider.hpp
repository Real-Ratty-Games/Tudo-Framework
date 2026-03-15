/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef COLLIDER_HPP_
#define COLLIDER_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	/// <summary>
	/// 2D AABB body
	/// </summary>
	struct RectCollider
	{
		vec2	Location;
		vec2	Size;
	};

	/// <summary>
	/// 2D Circle body
	/// </summary>
	struct CircleCollider
	{
		vec2	Location;
		float	Radius;
	};

	/// <summary>
	/// 3D AABB body
	/// </summary>
	struct BoxCollider
	{
		vec3 Min;
		vec3 Max;
	};

	/// <summary>
	/// 3D Sphere body
	/// </summary>
	struct SphereCollider
	{
		vec3	Location;
		float	Radius;
	};
}
#endif