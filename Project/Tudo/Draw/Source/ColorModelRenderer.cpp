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

ColorModelRenderer::ColorModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : ModelRenderer(gdevice, pipeline) {}

void ColorModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	Shader* shader = pPipeline->GetActiveShader();

	bgfx::setState(TUDO_RENDERER_MESH_DEFAULT_STATE);

	pGDevice->SetMesh(0, mesh);

	vec4 color = mColor.ToVec();
	shader->SetUniform("color", color.Ptr());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void ColorModelRenderer::SetColor(Color color)
{
	mColor = color;
}
