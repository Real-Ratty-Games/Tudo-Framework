/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawPipeline.hpp"
#include "BigError.hpp"
#include "Projection.hpp"
#include "GraphicsDevice.hpp"
#include "DrawSurface2D.hpp"
#include "DrawSurface3D.hpp"
#include "Viewport3D.hpp"

using namespace Tudo;

DrawPipeline::DrawPipeline(GraphicsDevice& gdevice) : DrawObject(gdevice)
{
	pActiveShader		= nullptr;
	pActiveDrawSurface	= nullptr;
}

void DrawPipeline::PrepareDrawModel(DrawSurface3D& surface, const Viewport3D& viewport)
{
	SetActiveDrawSurface(&surface);
	surface.Clear();
	mat4 proj = Math::ProjectPerspLH(Math::ToRadians(viewport.Fov), surface.AspectRatio, viewport.Near, viewport.Far);
	bgfx::setViewTransform(surface.ViewID(), viewport.View().Ptr(), proj.Ptr());
}

void DrawPipeline::PrepareDrawModel(DrawSurface3D& surface, const ViewportOrtho3D& viewport)
{
	SetActiveDrawSurface(&surface);
	surface.Clear();
	mat4 proj = Math::ProjectOrthoLH(viewport.Left, viewport.Right, viewport.Bottom, viewport.Top, viewport.Near,
		viewport.Far, viewport.Offset);
	bgfx::setViewTransform(surface.ViewID(), viewport.View().Ptr(), proj.Ptr());
}

void DrawPipeline::PrepareDrawSprite(DrawSurface2D& surface, const Viewport2D& viewport)
{
	SetActiveDrawSurface(&surface);
	surface.Clear();

	mat4 proj = Math::ProjectOrthoLH(viewport.Location.X, viewport.Size.X + viewport.Location.Y,
		viewport.Size.Y + viewport.Location.Y, viewport.Location.Y, 0.1f, 100.0f, 0.0f);

	bgfx::setViewTransform(surface.ViewID(), pGDevice->GetQuad2DView().Ptr(), proj.Ptr());
	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());
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
