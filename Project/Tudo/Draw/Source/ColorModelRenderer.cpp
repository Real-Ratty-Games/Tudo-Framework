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

ColorModelRenderer::ColorModelRenderer(GraphicsDevice* gdevice, DrawPipeline* pipeline) : ModelRenderer(gdevice, pipeline) {}

void ColorModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	Shader* shader = pPipeline->GetActiveShader();

	bgfx::setState(BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_BLEND_ALPHA
		| BGFX_STATE_CULL_CCW);

	pGDevice->SetMesh(0, mesh);

	vec4 color = mColor.ToVec();
	shader->SetUniform("color", color.Ptr());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_STATE, true);
}

void ColorModelRenderer::SetColor(Color color)
{
	mColor = color;
}
