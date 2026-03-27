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

BillboardRenderer::BillboardRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : Renderer(gdevice, pipeline) {}

void BillboardRenderer::Draw(Texture& texture, vec2 scale, Color color, bool cylindric)
{
	Shader* shader = pPipeline->GetActiveShader();

	bgfx::setState(TUDO_RENDERER_MESH_DEFAULT_STATE);

	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());

	vec4 scl = vec4(scale.X, scale.Y, cylindric ? 1.0f : 0.0f, 0.0f);
	shader->SetUniform("scale", scl.Ptr());

	shader->SetTexture(0, "s_texColor", texture);

	vec4 col = color.ToVec();
	shader->SetUniform("color", col.Ptr());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}
