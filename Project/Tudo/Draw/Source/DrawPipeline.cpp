/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawPipeline.hpp"
#include "BigError.hpp"
#include "Projection.hpp"
#include "GraphicsDevice.hpp"
#include "DrawSurface3D.hpp"
#include "Viewport3D.hpp"

using namespace Tudo;

DrawPipeline::DrawPipeline(GraphicsDevice* gdevice) : DrawObject(gdevice)
{
	pActiveShader		= nullptr;
	pActiveDrawSurface	= nullptr;
}

void DrawPipeline::PrepareDrawModel(DrawSurface3D* surface, Viewport3D& viewport)
{
	surface->Clear();
	mat4 proj = Math::ProjectPerspLH(Math::ToRadians(viewport.Fov), surface->AspectRatio, viewport.Near, viewport.Far);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj.Ptr());
}

void DrawPipeline::PrepareDrawModel(DrawSurface3D* surface, ViewportOrtho3D& viewport)
{
	surface->Clear();
	mat4 proj = Math::ProjectOrthoLH(viewport.Left, viewport.Right, viewport.Bottom, viewport.Top, viewport.Near,
		viewport.Far, viewport.Offset);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj.Ptr());
}

void DrawPipeline::SetActiveShader(Shader* shader)
{
	pActiveShader = shader;
}

void DrawPipeline::SetActiveDrawSurface(DrawSurface* surface)
{
	pActiveDrawSurface = surface;
}

Shader* DrawPipeline::GetActiveShader()
{
	if (!pActiveShader)
		throw BigError("Invalid Shader!");
	return pActiveShader;
}

DrawSurface* DrawPipeline::GetActiveDrawSurface()
{
	if (!pActiveDrawSurface)
		throw BigError("Invalid Surface!");
	return pActiveDrawSurface;
}
