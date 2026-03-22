/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "UnlitRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawSurface.hpp"
#include "Model3D.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

using namespace Tudo;

UnlitRenderer::UnlitRenderer(GraphicsDevice* gdevice) : ModelRenderer(gdevice)
{
	pTexture = nullptr;
}

void UnlitRenderer::DrawMesh(const Mesh3D& mesh)
{
	bgfx::setState(BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_BLEND_ALPHA
		| BGFX_STATE_CULL_CCW);

	pGDevice->SetMesh(0, mesh);
	pActiveShader->SetTexture(0, "s_texColor", *pTexture);
	pActiveShader->Submit(pActiveDrawSurface->ViewID(), TUDO_RENDERER_MESH_DEFAULT_STATE, true);
}

void UnlitRenderer::SetTexture(Texture* texture)
{
	pTexture = texture;
}
