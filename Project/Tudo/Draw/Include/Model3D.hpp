/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MODEL3D_HPP_
#define MODEL3D_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "DrawObject.hpp"

namespace Tudo
{
	class GraphicsDevice;

	class Model3D : public DrawObject
	{
	public:
		Model3D(GraphicsDevice& gdevice);
		~Model3D();
		const std::vector<Mesh3D>& Meshes() const;

		friend class AssetLoader;

	private:
		std::vector<Mesh3D>	mMeshes;
	};
}
#endif