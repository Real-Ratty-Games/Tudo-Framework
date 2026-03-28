/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWMATH_HPP_
#define DRAWMATH_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Viewport3D.hpp"

namespace Tudo::Math
{
	vec3 ScreenLocation2WorldLocation(vec2 screenLocation, vec2 screenSize,
		const Viewport3D& viewport3D, float distance);

	vec3 ScreenLocation2WorldLocationPY(vec2 screenLocation, vec2 screenSize,
		const Viewport3D& viewport3D, float yplane);

	vec3 ScreenLocation2WorldLocationPX(vec2 screenLocation, vec2 screenSize,
		const Viewport3D& viewport3D, float xplane);

	vec3 ScreenLocation2WorldLocationPZ(vec2 screenLocation, vec2 screenSize,
		const Viewport3D& viewport3D, float zplane);

	vec3 ScreenLocation2WorldLocation(vec2 screenLocation, vec2 screenSize,
		const ViewportOrtho3D& viewport3D);

	vec3 ScreenLocation2WorldLocationPY(vec2 screenLocation, vec2 screenSize,
		const ViewportOrtho3D& viewport3D, float yplane);

	vec3 ScreenLocation2WorldLocationPX(vec2 screenLocation, vec2 screenSize,
		const ViewportOrtho3D& viewport3D, float xplane);

	vec3 ScreenLocation2WorldLocationPZ(vec2 screenLocation, vec2 screenSize,
		const ViewportOrtho3D& viewport3D, float zplane);

	vec2 WorldLocation2ScreenLocation(const Viewport3D& viewport3D, vec2 screenSize, vec3 worldLocation);
}
#endif