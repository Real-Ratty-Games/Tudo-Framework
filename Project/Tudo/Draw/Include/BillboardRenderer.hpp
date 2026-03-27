/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef BILLBOARDRENDERER_HPP_
#define BILLBOARDRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Renderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class Texture;

	class BillboardRenderer : public Renderer
	{
	public:
		BillboardRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);
		void Draw(Texture& texture, vec2 scale, Color color, bool cylindric);
	};
}
#endif