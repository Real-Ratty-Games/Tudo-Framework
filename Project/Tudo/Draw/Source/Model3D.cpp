/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Model3D.hpp"
#include "Renderer.hpp"

using namespace Tudo;

Model3D::Model3D(Renderer* renderer) : DrawObject(renderer) {}

Model3D::~Model3D()
{
	for (auto& it : mMeshes)
	{
		if (bgfx::isValid(it.VBH))
			bgfx::destroy(it.VBH);
		if (bgfx::isValid(it.IBH))
			bgfx::destroy(it.IBH);
	}
}

const std::vector<Mesh3D>& Model3D::Meshes()
{
	return mMeshes;
}
