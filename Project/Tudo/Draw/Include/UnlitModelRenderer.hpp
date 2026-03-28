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
	class Shader;
	struct Mesh3D;

	class UnlitModelRenderer : public ModelRenderer
	{
	public:
		UnlitModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);
		void DrawMesh(const Mesh3D& mesh);
		void SetTexture(Texture* texture);

	protected:
		void SetupMesh() override;

	private:
		Texture*	pTexture;
		Shader*		pShader;
	};
}
#endif