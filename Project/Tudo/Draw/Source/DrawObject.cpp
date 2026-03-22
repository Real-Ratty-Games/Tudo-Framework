/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawObject.hpp"
#include "BigError.hpp"
#include "GraphicsDevice.hpp"

using namespace Tudo;

DrawObject::DrawObject(GraphicsDevice* gdevice)
{
	if (!gdevice)
		throw BigError("Instance requires a GraphicsDevice!");
	pGDevice = gdevice;
}

GraphicsDevice* DrawObject::GetRenderer()
{
	return pGDevice;
}
