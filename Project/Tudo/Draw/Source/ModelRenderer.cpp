/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "ModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "Model3D.hpp"

using namespace Tudo;

ModelRenderer::ModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : Renderer(gdevice, pipeline) {}

void ModelRenderer::DrawModel(const Model3D& model)
{
	SetupMesh();
	for (auto& mesh : model.Meshes())
		DrawMesh(mesh);
}

void ModelRenderer::DrawModelInstanced(const ModelInstanceData& idata)
{
	bgfx::setInstanceDataBuffer(&idata.Buffer);
	
	SetupMeshInstanced();
	for (auto& mesh : idata.pModel->Meshes())
		DrawMeshInstanced(mesh);
}

void ModelRenderer::SetupMeshInstanced()
{
	SetupMesh();
}

void ModelRenderer::DrawMeshInstanced(const Mesh3D& mesh)
{
	DrawMesh(mesh);
}
