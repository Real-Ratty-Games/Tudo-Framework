/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Collision.hpp"
#include <algorithm>

using namespace Tudo::Math;

bool Collision::Intersect(vec2 point, const RectCollider& rect)
{
	return point.X >= rect.Location.X && point.X <= rect.Location.X + rect.Size.X &&
		point.Y >= rect.Location.Y && point.Y <= rect.Location.Y + rect.Size.Y;
}

bool Collision::Intersect(vec2 point, const CircleCollider& circle)
{
	float dx = point.X - circle.Location.X;
	float dy = point.Y - circle.Location.Y;
	return (dx * dx + dy * dy) <= (circle.Radius * circle.Radius);
}

bool Collision::Intersect(const RectCollider& a, const RectCollider& b)
{
	bool cX = a.Location.X + a.Size.X >= b.Location.X &&
		b.Location.X + b.Size.X >= a.Location.X;
	bool cY = a.Location.Y + a.Size.Y >= b.Location.Y &&
		b.Location.Y + b.Size.Y >= a.Location.Y;
	return cX && cY;
}

bool Collision::Intersect(const CircleCollider& a, const CircleCollider& b)
{
	float dx = a.Location.X - b.Location.X;
	float dy = a.Location.Y - b.Location.Y;
	float r = a.Radius + b.Radius;
	return (dx * dx + dy * dy) <= (r * r);
}

bool Collision::Intersect(const RectCollider& rect, const CircleCollider& circle)
{
	float closestX = std::clamp(
		circle.Location.X,
		rect.Location.X,
		rect.Location.X + rect.Size.X
	);

	float closestY = std::clamp(
		circle.Location.Y,
		rect.Location.Y,
		rect.Location.Y + rect.Size.Y
	);

	float dx = circle.Location.X - closestX;
	float dy = circle.Location.Y - closestY;

	return (dx * dx + dy * dy) <= (circle.Radius * circle.Radius);
}

bool Collision::Intersect(const BoxCollider& a, const BoxCollider& b)
{
	return(a.Max.X > b.Min.X &&
		a.Min.X < b.Max.X &&
		a.Max.Y > b.Min.Y &&
		a.Min.Y < b.Max.Y &&
		a.Max.Z > b.Min.Z &&
		a.Min.Z < b.Max.Z);
}

bool Collision::Intersect(vec3 point, const BoxCollider& box)
{
	return (point.X > box.Min.X && point.X < box.Max.X &&
		point.Y > box.Min.Y && point.Y < box.Max.Y &&
		point.Z > box.Min.Z && point.Z < box.Max.Z);
}

bool Collision::Intersect(vec3 point, const SphereCollider& sphere)
{
	const vec3 dist = sphere.Location - point;
	const float distSq = vec3::Dot(dist, dist);
	return (distSq < (sphere.Radius * sphere.Radius));
}

bool Collision::Intersect(const SphereCollider& a, const SphereCollider& b)
{
	const vec3 dist = b.Location - a.Location;
	const float distSq = vec3::Dot(dist, dist);

	float sumSq = a.Radius + b.Radius;
	sumSq *= sumSq;

	return (distSq <= sumSq);
}

bool Collision::Intersect(const BoxCollider& box, const SphereCollider& sphere)
{
	const float xx = Maxi(box.Min.X, Mini(sphere.Location.X, box.Max.X));
	const float yy = Maxi(box.Min.Y, Mini(sphere.Location.Y, box.Max.Y));
	const float zz = Maxi(box.Min.Z, Mini(sphere.Location.Z, box.Max.Z));

	const float dx = xx - sphere.Location.X;
	const float dy = yy - sphere.Location.Y;
	const float dz = zz - sphere.Location.Z;

	return ((dx * dx + dy * dy + dz * dz) <= (sphere.Radius * sphere.Radius));
}
