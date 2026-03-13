/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"

namespace GameEngine
{
	class Renderer;

	class Texture : public DrawObject
	{
	public:
		Texture(Renderer* renderer);
		~Texture();

		// For uncompressed textures
		void LoadFromFile(strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen);
		void LoadFromMemory(std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen);

		// For GPU friendly textures
		// DDS on Windows and KTX on macOS
		void LoadFromFileGPU(strgv filename, uint64 flags, strgv texturename);
		void LoadFromMemoryGPU(std::vector<uint8>& memData, uint64 flags, strgv texturename);

		bgfx::TextureHandle&	Handle();
		const vec2i&			Size();

		friend class DrawSurface2D;
		friend class DrawSurface3D;

	private:
		void LoadTexture(uint8* data, uint64 flags, int nrComponents,
			strgv texturename, int width, int height, bool mipgen);
		void LoadGPUTexture(std::vector<uint8>& data, uint64 flags, strgv texturename);

	private:
		bgfx::TextureHandle mHandle;
		vec2i				mSize;
	};
}
#endif