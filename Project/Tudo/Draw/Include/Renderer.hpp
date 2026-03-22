/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef RENDERER_HPP_
#define RENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;

	class Renderer : public DrawObject
	{
	public:
		Renderer(GraphicsDevice* gdevice, DrawPipeline* pipeline);

	protected:
		DrawPipeline* pPipeline;
	};
}
#endif