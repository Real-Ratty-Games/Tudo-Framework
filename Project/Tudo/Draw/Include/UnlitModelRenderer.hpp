/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef UNLITMODELRENDERER_HPP_
#define UNLITMODELRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "ModelRenderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class Texture;
	struct Mesh3D;

	class UnlitModelRenderer : public ModelRenderer
	{
	public:
		UnlitModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);
		void DrawMesh(const Mesh3D& mesh);
		void SetTexture(Texture* texture);

	private:
		Texture* pTexture;
	};
}
#endif