/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef UNLITRENDERER_HPP_
#define UNLITRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "ModelRenderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class Texture;
	class Mesh3D;

	class UnlitRenderer : public ModelRenderer
	{
	public:
		UnlitRenderer(GraphicsDevice* gdevice);
		void DrawMesh(const Mesh3D& mesh);
		void SetTexture(Texture* texture);

	private:
		Texture* pTexture;
	};
}
#endif