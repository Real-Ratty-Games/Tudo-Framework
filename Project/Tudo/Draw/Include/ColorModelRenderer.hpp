/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef COLORMODELENDERER_HPP_
#define COLORMODELENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "ModelRenderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class Shader;
	struct Mesh3D;

	class ColorModelRenderer : public ModelRenderer
	{
	public:
		ColorModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);
		void DrawMesh(const Mesh3D& mesh);
		void SetColor(Color color);

	protected:
		void SetupMesh() override;
		void DrawMeshInstanced(const Mesh3D& mesh) override;

	private:
		Color	mColor;
		Shader* pShader;
	};
}
#endif