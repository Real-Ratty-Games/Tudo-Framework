/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"

namespace Tudo
{
	class Renderer;

	class Texture : public DrawObject
	{
	public:
		Texture(Renderer* renderer);
		~Texture();

		bgfx::TextureHandle&	Handle();
		const vec2i&			Size();

		friend class DrawSurface2D;
		friend class DrawSurface3D;
		friend class AssetLoader;

	private:
		bgfx::TextureHandle mHandle;
		vec2i				mSize;
	};
}
#endif