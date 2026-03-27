/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawMath.hpp"
#include "Renderer.hpp"
#include "Transformation.hpp"
#include "Multiply.hpp"
#include "Projection.hpp"

using namespace Tudo;

static inline void DrawMath_ScreenLoc2WorldLoc(vec2 screenLocation, vec2 screenSize, const Viewport3D& viewport3D,
	vec3& screenRay, vec4& rayWorld);

vec3 Math::ScreenLocation2WorldLocation(vec2 screenLocation, vec2 screenSize,
	const Viewport3D& viewport3D, float distance)
{
	vec3 screenRay;
	vec4 rayWorld;
	DrawMath_ScreenLoc2WorldLoc(screenLocation, screenSize, viewport3D, screenRay, rayWorld);
	return viewport3D.Eye + screenRay * distance;
}

vec3 Math::ScreenLocation2WorldLocationPY(vec2 screenLocation, vec2 screenSize,
	const Viewport3D& viewport3D, float yplane)
{
	vec3 screenRay;
	vec4 rayWorld;
	DrawMath_ScreenLoc2WorldLoc(screenLocation, screenSize, viewport3D, screenRay, rayWorld);

	vec3 rayDir = vec3(rayWorld.X, rayWorld.Y, rayWorld.Z).Normalized();
	float t = (yplane - viewport3D.Eye.Y) / rayDir.Y;
	vec3 worldPos = viewport3D.Eye + rayDir * t;
	return worldPos;
}

vec3 Math::ScreenLocation2WorldLocationPX(vec2 screenLocation, vec2 screenSize,
	const Viewport3D& viewport3D, float xplane)
{
	vec3 screenRay;
	vec4 rayWorld;
	DrawMath_ScreenLoc2WorldLoc(screenLocation, screenSize, viewport3D, screenRay, rayWorld);

	vec3 rayDir = vec3(rayWorld.X, rayWorld.Y, rayWorld.Z).Normalized();
	float t = (xplane - viewport3D.Eye.X) / rayDir.X;
	vec3 worldPos = viewport3D.Eye + rayDir * t;
	return worldPos;
}

vec3 Math::ScreenLocation2WorldLocationPZ(vec2 screenLocation, vec2 screenSize,
	const Viewport3D& viewport3D, float zplane)
{
	vec3 screenRay;
	vec4 rayWorld;
	DrawMath_ScreenLoc2WorldLoc(screenLocation, screenSize, viewport3D, screenRay, rayWorld);

	vec3 rayDir = vec3(rayWorld.X, rayWorld.Y, rayWorld.Z).Normalized();
	float t = (zplane - viewport3D.Eye.Z) / rayDir.Z;
	vec3 worldPos = viewport3D.Eye + rayDir * t;
	return worldPos;
}

vec3 Math::ScreenLocation2WorldLocation(vec2 screenLocation, vec2 screenSize,
	const ViewportOrtho3D& viewport3D)
{
	const float glX = (2.0f * screenLocation.X) / screenSize.X - 1.0f;
	const float glY = -((2.0f * screenLocation.Y) / screenSize.Y - 1.0f);
	vec3 world;

	world.X = viewport3D.Left + (glX + 1.0f) * 0.5f * (viewport3D.Right - viewport3D.Left);
	world.Y = viewport3D.Bottom + (glY + 1.0f) * 0.5f * (viewport3D.Top - viewport3D.Bottom);
	world.Z = viewport3D.Eye.Z;
	return world;
}

vec3 Math::ScreenLocation2WorldLocationPY(vec2 screenLocation, vec2 screenSize,
	const ViewportOrtho3D& viewport3D, float yplane)
{
	const float glX = (2.0f * screenLocation.X) / screenSize.X - 1.0f;
	const float glY = -((2.0f * screenLocation.Y) / screenSize.Y - 1.0f);
	vec3 world;

	world.X = viewport3D.Left + (glX + 1.0f) * 0.5f * (viewport3D.Right - viewport3D.Left);
	world.Y = yplane;
	world.Z = viewport3D.Bottom + (glY + 1.0f) * 0.5f * (viewport3D.Top - viewport3D.Bottom);
	return world;
}

vec3 Math::ScreenLocation2WorldLocationPX(vec2 screenLocation, vec2 screenSize,
	const ViewportOrtho3D& viewport3D, float xplane)
{
	const float glX = (2.0f * screenLocation.X) / screenSize.X - 1.0f;
	const float glY = -((2.0f * screenLocation.Y) / screenSize.Y - 1.0f);
	vec3 world;

	world.X = xplane;
	world.Y = viewport3D.Bottom + (glY + 1.0f) * 0.5f * (viewport3D.Top - viewport3D.Bottom);
	world.Z = viewport3D.Left + (glX + 1.0f) * 0.5f * (viewport3D.Right - viewport3D.Left);
	return world;
}

vec3 Math::ScreenLocation2WorldLocationPZ(vec2 screenLocation, vec2 screenSize,
	const ViewportOrtho3D& viewport3D, float zplane)
{
	const float glX = (2.0f * screenLocation.X) / screenSize.X - 1.0f;
	const float glY = -((2.0f * screenLocation.Y) / screenSize.Y - 1.0f);
	vec3 world;

	world.X = viewport3D.Left + (glX + 1.0f) * 0.5f * (viewport3D.Right - viewport3D.Left);
	world.Y = viewport3D.Bottom + (glY + 1.0f) * 0.5f * (viewport3D.Top - viewport3D.Bottom);
	world.Z = zplane;
	return world;
}

/*======================================================
======================================================*/

inline void DrawMath_ScreenLoc2WorldLoc(vec2 screenLocation, vec2 screenSize, const Viewport3D& viewport3D,
	vec3& screenRay, vec4& rayWorld)
{
	const float glCoordX = (2.0f * screenLocation.X) / screenSize.X - 1.0f;
	const float glCoordY = -((2.0f * screenLocation.Y) / screenSize.Y - 1.0f);
	vec4 clipCoords = vec4(glCoordX, glCoordY, 1.0f, 1.0f);

	mat4 proj = Math::ProjectPerspLH(viewport3D.Fov, screenSize.X / screenSize.Y, viewport3D.Near, viewport3D.Far);

	mat4 invertedProjection;
	if (!proj.Inverse(invertedProjection))
		invertedProjection = proj;

	vec4 eyeCoord = Math::Multiply(clipCoords, invertedProjection);
	eyeCoord = eyeCoord / eyeCoord.W;

	mat4 view = viewport3D.View();
	mat4 invertedView;
	if (!view.Inverse(invertedView))
		invertedView = view;

	rayWorld = Math::Multiply(eyeCoord, invertedView);
	screenRay = vec3(rayWorld.X, rayWorld.Y, rayWorld.Z);
	screenRay = screenRay.Normalized();
}
