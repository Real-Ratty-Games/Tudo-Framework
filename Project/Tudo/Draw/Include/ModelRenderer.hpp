/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MODELRENDERER_HPP_
#define MODELRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Renderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class Model3D;

	class ModelRenderer : public Renderer
	{
	public:
		ModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);

		void			DrawModel(const Model3D& model);
		virtual void	DrawMesh(const Mesh3D& mesh) = 0;
	};
}
#endif