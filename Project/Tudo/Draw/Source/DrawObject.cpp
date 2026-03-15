/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawObject.hpp"
#include "BigError.hpp"
#include "Renderer.hpp"

using namespace Tudo;

DrawObject::DrawObject(Renderer* renderer)
{
	if (!renderer)
		throw BigError("Instance requires a renderer!");
	pRenderer = renderer;
}

Renderer* DrawObject::GetRenderer()
{
	return pRenderer;
}
