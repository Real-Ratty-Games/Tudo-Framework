/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "ModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "Model3D.hpp"

using namespace Tudo;

ModelRenderer::ModelRenderer(GraphicsDevice* gdevice, DrawPipeline* pipeline) : Renderer(gdevice, pipeline) {}

void ModelRenderer::DrawModel(const Model3D& model)
{
	for (auto& mesh : model.Meshes())
		DrawMesh(mesh);
}
