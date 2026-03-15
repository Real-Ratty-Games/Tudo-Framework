/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawSurface3D.hpp"
#include <bx/bx.h>
#include <bx/math.h>

using namespace Tudo;

DrawSurface3D::DrawSurface3D(Renderer* renderer, uint16 viewid, vec2 size, void* wndHandle, bool depthOnly) : DrawSurface(renderer, viewid, size, wndHandle)
{
	bDepthOnly = depthOnly;
	if (viewid != 0)
	{
		UpdateFB(size);
		SetFBViewId();
	}
}

Texture* DrawSurface3D::GetDepthTexture()
{
	return mFbDepthTex.Get();
}

void DrawSurface3D::UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format)
{
	if (!bgfx::isValid(mFbHandle))
	{
		if (pWindowHandle != nullptr)
			mFbHandle = bgfx::createFrameBuffer(pWindowHandle, texSize.X, texSize.Y, format, bgfx::TextureFormat::D32F);
		else
		{
			if (bDepthOnly)
			{
				mFbDepthTex->mHandle = bgfx::createTexture2D(
					(uint16)texSize.X
					, (uint16)texSize.Y
					, false
					, 1
					, bgfx::TextureFormat::D32F
					, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
				);
				mFbDepthTex->mSize = texSize;

				mFbHandle = bgfx::createFrameBuffer(1, &mFbDepthTex->mHandle, true);
			}
			else
			{
				bgfx::TextureHandle fbtextures[] =
				{
					bgfx::createTexture2D(
						(uint16)texSize.X
						, (uint16)texSize.Y
						, false
						, 1
						, format
						, BGFX_TEXTURE_RT
					),

					bgfx::createTexture2D(
						(uint16)texSize.X
						, (uint16)texSize.Y
						, false
						, 1
						, bgfx::TextureFormat::D32F
						, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
					)
				};

				mFbTex->mHandle = fbtextures[0];
				mFbTex->mSize = texSize;

				mFbDepthTex->mHandle = fbtextures[1];
				mFbDepthTex->mSize = texSize;

				mFbHandle = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
			}
		}
	}
}
