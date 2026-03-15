/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef COLLISION_HPP_
#define COLLISION_HPP_
#include "SystemTypes.hpp"
#include "Collider.hpp"

namespace Tudo::Math::Collision
{
	// 2D

	bool Intersect(const vec2& point,			const RectCollider& rect);
	bool Intersect(const vec2& point,			const CircleCollider& circle);
	bool Intersect(const RectCollider& a,		const RectCollider& b);
	bool Intersect(const CircleCollider& a,		const CircleCollider& b);
	bool Intersect(const RectCollider& rect,	const CircleCollider& circle);

	// 3D

	bool Intersect(const BoxCollider& a,		const BoxCollider& b);
	bool Intersect(const vec3& point,			const BoxCollider& box);
	bool Intersect(const vec3& point,			const SphereCollider& sphere);
	bool Intersect(const SphereCollider& a,		const SphereCollider& b);
	bool Intersect(const BoxCollider& box,		const SphereCollider& sphere);
}
#endif