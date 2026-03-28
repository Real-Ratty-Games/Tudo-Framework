/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "ColorModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "DrawSurface.hpp"
#include "Shader.hpp"

using namespace Tudo;

ColorModelRenderer::ColorModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : ModelRenderer(gdevice, pipeline)
{
	pShader = nullptr;
}

void ColorModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	bgfx::setState(TUDO_RENDERER_MESH_DEFAULT_STATE);

	vec4 color = mColor.ToVec();
	pGDevice->SetShaderUniform("u_color", color.Ptr());

	pGDevice->SetMesh(0, mesh);
	pShader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void ColorModelRenderer::SetupMesh()
{
	pShader = pPipeline->GetActiveShader();
}

void ColorModelRenderer::DrawMeshInstanced(const Mesh3D& mesh)
{
	bgfx::setState(TUDO_RENDERER_MESH_DEFAULT_STATE);
	pGDevice->SetMesh(0, mesh);
	pShader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void ColorModelRenderer::SetColor(Color color)
{
	mColor = color;
}
