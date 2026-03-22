/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWSURFACE_HPP_
#define DRAWSURFACE_HPP_
#include "SystemTypes.hpp"
#include "Memory.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class Texture;

	class DrawSurface : public DrawObject
	{
	public:
		DrawSurface(GraphicsDevice* gdevice, uint16 viewid, vec2 size, void* wndHandle);
		~DrawSurface();
		void Clear();
		void OnResize(vec2 size);
		uint16 ViewID() const;
		Texture* GetTexture();

	protected:
		void DestroyFB();
		void SetFBViewId();
		virtual void UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8) = 0;

	public:
		vec2i					Location;
		vec2					Resolution;
		float					AspectRatio;
		Color					ClearColor;
		bool					bTopMost;
		bool					bTransparent;

	protected:
		bgfx::FrameBufferHandle mFbHandle;
		SafePtr<Texture>		mFbTex;

		uint16					mViewId;
		void*					pWindowHandle;
	};
}
#endif