/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Texture.hpp"
#include "GraphicsDevice.hpp"

using namespace Tudo;

Texture::Texture(GraphicsDevice* gdevice) : DrawObject(gdevice)
{
	mHandle = BGFX_INVALID_HANDLE;
}

Texture::~Texture()
{
	if (bgfx::isValid(mHandle))
	{
		bgfx::destroy(mHandle);
		mHandle = BGFX_INVALID_HANDLE;
	}
}

bgfx::TextureHandle& Texture::Handle()
{
	return mHandle;
}

const vec2i& Texture::Size()
{
	return mSize;
}
