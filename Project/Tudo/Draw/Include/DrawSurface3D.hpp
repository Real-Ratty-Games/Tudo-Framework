/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWSURFACE3D_HPP_
#define DRAWSURFACE3D_HPP_
#include "SystemTypes.hpp"
#include "Memory.hpp"
#include "DrawData.hpp"
#include "DrawSurface.hpp"
#include "Texture.hpp"

namespace Tudo
{
	class Renderer;

	class DrawSurface3D : public DrawSurface
	{
	public:
		DrawSurface3D(Renderer* renderer, uint16 viewid, vec2 size, void* wndHandle, bool depthOnly);
		Texture* GetDepthTexture();

	protected:
		void UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8) override;

	private:
		SafePtr<Texture>	mFbDepthTex;
		bool				bDepthOnly;
	};
}
#endif