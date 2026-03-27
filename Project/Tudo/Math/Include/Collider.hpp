/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef COLLIDER_HPP_
#define COLLIDER_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	/// 2D AABB body
	struct RectCollider
	{
		vec2	Location;
		vec2	Size;
	};

	/// 2D Circle body
	struct CircleCollider
	{
		vec2	Location;
		float	Radius;
	};

	/// 3D AABB body
	struct BoxCollider
	{
		vec3 Min;
		vec3 Max;
	};

	/// 3D Sphere body
	struct SphereCollider
	{
		vec3	Location;
		float	Radius;
	};
}
#endif