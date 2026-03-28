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
#include "Model3D.hpp"

using namespace Tudo;

DrawPipeline::DrawPipeline(GraphicsDevice& gdevice) : DrawObject(gdevice)
{
	pActiveShader		= nullptr;
	pActiveDrawSurface	= nullptr;
}

void DrawPipeline::DrawAll()
{
	Draw();
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

void DrawPipeline::PrepareModelInstancing(Model3D& model, ModelInstanceData& idata, std::vector<ModelInstanceTransform>& tdata)
{
	const int insCnt = (int)tdata.size();
	const uint16 insStride = sizeof(mat4) + sizeof(vec4);
	uint drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	idata.MissedAmount = insCnt - drawnIns;
	idata.pModel = &model;

	bgfx::allocInstanceDataBuffer(&idata.Buffer, drawnIns, insStride);

	uint8* data = idata.Buffer.data;

	for (uint i = 0; i < drawnIns; i++)
	{
		auto& transf = tdata[i];

		Memory::Copy(data, transf.ModelMatrix.Ptr(), sizeof(mat4));
		data += sizeof(mat4);

		Memory::Copy(data, transf.IData.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
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
