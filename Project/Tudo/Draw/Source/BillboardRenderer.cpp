/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "BillboardRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "DrawSurface.hpp"

using namespace Tudo;

BillboardRenderer::BillboardRenderer(GraphicsDevice* gdevice, DrawPipeline* pipeline) : Renderer(gdevice, pipeline) {}

void BillboardRenderer::Draw(Texture* texture, vec2 scale, Color color, bool alt)
{
	Shader* shader = pPipeline->GetActiveShader();

	bgfx::setState(BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_BLEND_ALPHA
		| BGFX_STATE_CULL_CCW);

	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());

	vec4 scl = vec4(scale.X, scale.Y, alt ? 1.0f : 0.0f, 0.0f);
	shader->SetUniform("scale", scl.Ptr());

	shader->SetTexture(0, "s_texColor", *texture);

	vec4 col = color.ToVec();
	shader->SetUniform("color", col.Ptr());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_STATE, true);
}
