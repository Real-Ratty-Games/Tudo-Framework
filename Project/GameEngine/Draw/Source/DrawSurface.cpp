/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawSurface.hpp"
#include "Window.hpp"
#include <bx/bx.h>

using namespace GameEngine;

DrawSurface::DrawSurface(Renderer* renderer, uint16 viewid, vec2 size, void* wndHandle) : DrawObject(renderer)
{
	Location		= vec2i(0);
	Resolution		= size;
	AspectRatio		= size.X / size.Y;
	ClearColor		= 0xfff;
	bTopMost		= true;
	bTransparent	= false;

	mViewId = viewid;
	pWindowHandle = wndHandle;

	mFbTex = new Texture(renderer);

	bgfx::setViewMode(mViewId, bgfx::ViewMode::Sequential);

	mFbHandle = BGFX_INVALID_HANDLE;

	bgfx::setViewRect(mViewId, Location.X, Location.Y, static_cast<uint16>(Resolution.X), static_cast<uint16>(Resolution.Y));
	bgfx::setViewScissor(mViewId, Location.X, Location.Y, static_cast<uint16>(Resolution.X), static_cast<uint16>(Resolution.Y));
	Clear();
}

DrawSurface::~DrawSurface()
{
	if(mViewId != 0)
		DestroyFB();
}

void DrawSurface::Clear()
{
	uint16 flags = BGFX_CLEAR_NONE;
	if (bTopMost)
		flags |= BGFX_CLEAR_DEPTH;
	if (!bTransparent)
		flags |= BGFX_CLEAR_COLOR;

	bgfx::setViewClear(mViewId, flags, ClearColor, 1.0f, 0);
}

void DrawSurface::OnResize(vec2 size)
{
	Resolution = size;
	AspectRatio = Resolution.X / Resolution.Y;

	if (bgfx::isValid(mFbHandle))
	{
		DestroyFB();
		UpdateFB(Resolution);
		SetFBViewId();
	}

	bgfx::setViewRect(mViewId, Location.X, Location.Y, static_cast<uint16>(Resolution.X), static_cast<uint16>(Resolution.Y));
	bgfx::setViewScissor(mViewId, Location.X, Location.Y, static_cast<uint16>(Resolution.X), static_cast<uint16>(Resolution.Y));
}

uint16 DrawSurface::ViewID() const
{
	return mViewId;
}

Texture* DrawSurface::GetTexture()
{
	return mFbTex.Get();
}

void DrawSurface::SetFBViewId()
{
	bgfx::setViewFrameBuffer(mViewId, mFbHandle);
}

void DrawSurface::DestroyFB()
{
	if (bgfx::isValid(mFbHandle))
	{
		bgfx::destroy(mFbHandle);
		mFbHandle.idx = bgfx::kInvalidHandle;
		bgfx::setViewFrameBuffer(mViewId, BGFX_INVALID_HANDLE);
	}
}
