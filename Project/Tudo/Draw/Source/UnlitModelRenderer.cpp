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

UnlitModelRenderer::UnlitModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : ModelRenderer(gdevice, pipeline)
{
	pTexture	= nullptr;
	pShader		= nullptr;
}

void UnlitModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	bgfx::setState(TUDO_RENDERER_MESH_DEFAULT_STATE);
	pGDevice->SetShaderTexture(0, "s_texColor", *pTexture);
	pGDevice->SetMesh(0, mesh);
	pShader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void UnlitModelRenderer::SetupMesh()
{
	pShader = pPipeline->GetActiveShader();
}

void UnlitModelRenderer::SetTexture(Texture* texture)
{
	pTexture = texture;
}
