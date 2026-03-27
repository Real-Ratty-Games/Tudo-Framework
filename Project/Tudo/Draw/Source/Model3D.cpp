/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Model3D.hpp"
#include "GraphicsDevice.hpp"

using namespace Tudo;

Model3D::Model3D(GraphicsDevice& gdevice) : DrawObject(gdevice) {}

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

const std::vector<Mesh3D>& Model3D::Meshes() const
{
	return mMeshes;
}
