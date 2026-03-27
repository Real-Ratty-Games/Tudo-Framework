/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWSURFACE2D_HPP_
#define DRAWSURFACE2D_HPP_
#include "SystemTypes.hpp"
#include "DrawSurface.hpp"

namespace Tudo
{
	class GraphicsDevice;

	class DrawSurface2D : public DrawSurface
	{
	public:
		DrawSurface2D(GraphicsDevice& gdevice, uint16 viewid, vec2 size, void* wndHandle);

	protected:
		void UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8);
	};
}
#endif