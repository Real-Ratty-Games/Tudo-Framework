/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "UnlitModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "DrawSurface.hpp"
#include "Model3D.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

using namespace Tudo;

UnlitModelRenderer::UnlitModelRenderer(GraphicsDevice* gdevice, DrawPipeline* pipeline) : ModelRenderer(gdevice, pipeline)
{
	pTexture = nullptr;
}

void UnlitModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	Shader* shader = pPipeline->GetActiveShader();

	bgfx::setState(BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_BLEND_ALPHA
		| BGFX_STATE_CULL_CCW);

	pGDevice->SetMesh(0, mesh);
	shader->SetTexture(0, "s_texColor", *pTexture);
	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_STATE, true);
}

void UnlitModelRenderer::SetTexture(Texture* texture)
{
	pTexture = texture;
}
