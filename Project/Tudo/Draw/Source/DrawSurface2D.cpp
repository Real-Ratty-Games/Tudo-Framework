/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawSurface2D.hpp"
#include "GraphicsDevice.hpp"
#include "Texture.hpp"

using namespace Tudo;

DrawSurface2D::DrawSurface2D(GraphicsDevice& gdevice, uint16 viewid, vec2 size, void* wndHandle) : DrawSurface(gdevice, viewid, size, wndHandle)
{
	if (viewid != 0)
	{
		UpdateFB(size);
		SetFBViewId();
	}

	bgfx::setViewMode(mViewId, bgfx::ViewMode::Sequential);
}

void DrawSurface2D::UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format)
{
	if (!bgfx::isValid(mFbHandle))
	{
		if (pWindowHandle != nullptr)
			mFbHandle = bgfx::createFrameBuffer(pWindowHandle, (uint16)texSize.X, (uint16)texSize.Y, format, bgfx::TextureFormat::D32F);
		else
		{
			mFbTex->mHandle = bgfx::createTexture2D(
				(uint16)texSize.X,
				(uint16)texSize.Y,
				false,
				1,
				format,
				BGFX_TEXTURE_RT
			);

			mFbTex->mSize = texSize;
			mFbHandle = bgfx::createFrameBuffer(1, &mFbTex->mHandle, true);
		}
	}
}
