/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "DrawSurface.hpp"
#include "GraphicsDevice.hpp"
#include "Window.hpp"
#include "Texture.hpp"

using namespace Tudo;

DrawSurface::DrawSurface(GraphicsDevice* gdevice, uint16 viewid, vec2 size, void* wndHandle) : DrawObject(gdevice)
{
	Location		= vec2i(0);
	Resolution		= size;
	AspectRatio		= size.X / size.Y;
	ClearColor		= 0;
	bTopMost		= true;
	bTransparent	= false;

	mViewId = viewid;
	pWindowHandle = wndHandle;

	mFbTex = new Texture(gdevice);

	bgfx::setViewMode(mViewId, bgfx::ViewMode::Sequential);

	mFbHandle = BGFX_INVALID_HANDLE;

	bgfx::setViewRect(mViewId, (uint16)Location.X, (uint16)Location.Y, (uint16)Resolution.X, (uint16)Resolution.Y);
	bgfx::setViewScissor(mViewId, (uint16)Location.X, (uint16)Location.Y, (uint16)Resolution.X, (uint16)Resolution.Y);
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

	bgfx::setViewClear(mViewId, flags, ClearColor.ToInt());
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

	bgfx::setViewRect(mViewId, (uint16)Location.X, (uint16)Location.Y, (uint16)Resolution.X, (uint16)Resolution.Y);
	bgfx::setViewScissor(mViewId, (uint16)Location.X, (uint16)Location.Y, (uint16)Resolution.X, (uint16)Resolution.Y);
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
