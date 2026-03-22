/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Renderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawSurface.hpp"
#include "Shader.hpp"

using namespace Tudo;

Renderer::Renderer(GraphicsDevice* gdevice) : DrawObject(gdevice)
{
	ClearActives();
}

void Renderer::SetActiveShader(Shader* shader)
{
	pActiveShader = shader;
}

void Renderer::SetActiveSurface(DrawSurface* surface)
{
	pActiveDrawSurface = surface;
}

void Renderer::ClearActives()
{
	pActiveShader		= nullptr;
	pActiveDrawSurface	= nullptr;
}
