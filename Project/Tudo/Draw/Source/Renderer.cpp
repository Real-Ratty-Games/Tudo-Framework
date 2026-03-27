/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Renderer.hpp"
#include "BigError.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"

using namespace Tudo;

Renderer::Renderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : DrawObject(gdevice)
{
	pPipeline = &pipeline;
}
