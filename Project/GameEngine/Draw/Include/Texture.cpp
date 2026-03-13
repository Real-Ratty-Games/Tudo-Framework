/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Texture.hpp"
#include "Renderer.hpp"
#include "BigError.hpp"
#include <bx/math.h>
#include <bx/allocator.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

using namespace GameEngine;

Texture::Texture(Renderer* renderer) : DrawObject(renderer)
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

void Texture::LoadFromFile(strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load(filename.data(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from file: " + strg(filename);
		throw BigError(errmsg);
	}

	LoadTexture(data, flags, nrComponents, texturename, width, height, mipgen);
}

void Texture::LoadFromMemory(std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load_from_memory(memData.data(), memData.size(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from memory: " + strg(texturename);
		throw BigError(errmsg);
	}

	LoadTexture(data, flags, nrComponents, texturename, width, height, mipgen);
}

void Texture::LoadFromFileGPU(strgv filename, uint64 flags, strgv texturename)
{
	std::ifstream file(filename.data(), std::ios::binary);
	if (!file)
	{
		const strg errmsg = "Failed loading GPU texture from file: " + strg(filename);
		throw BigError(errmsg);
	}

	file.seekg(0, std::ios::end);
	const uint64 size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8> data(size);
	file.read(reinterpret_cast<char*>(data.data()), size);
	file.close();

	LoadGPUTexture(data, flags, texturename);
}

void Texture::LoadFromMemoryGPU(std::vector<uint8>& memData, uint64 flags, strgv texturename)
{
	LoadGPUTexture(memData, flags, texturename);
}

bgfx::TextureHandle& Texture::Handle()
{
	return mHandle;
}

const vec2i& Texture::Size()
{
	return mSize;
}

void Texture::LoadTexture(uint8* data, uint64 flags, int nrComponents,
	strgv texturename, int width, int height, bool mipgen)
{
	bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
	if (nrComponents == 3)
		format = bgfx::TextureFormat::RGB8;
	else if (nrComponents == 4)
		format = bgfx::TextureFormat::RGBA8;

	if (mipgen)
	{
		int nmips = std::max(width, height);
		nmips = 1 + static_cast<int>(std::floor(std::log2(nmips)));

		mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			true,
			1,
			format,
			flags,
			nullptr);

		int mipWidth = width;
		int mipHeight = height;
		stbi_uc* mipData = data;

		for (int i = 0; i < nmips; i++)
		{
			bgfx::updateTexture2D(
				mHandle,
				0,
				i,
				0, 0,
				static_cast<uint16>(mipWidth),
				static_cast<uint16>(mipHeight),
				bgfx::copy(mipData, mipWidth * mipHeight * nrComponents));

			mipWidth = std::max(1, mipWidth / 2);
			mipHeight = std::max(1, mipHeight / 2);
			stbi_uc* nextMip = (stbi_uc*)malloc(mipWidth * mipHeight * nrComponents);

			stbir_resize_uint8_linear(mipData, mipWidth * 2, mipHeight * 2, 0, nextMip, mipWidth, mipHeight, 0,
				(stbir_pixel_layout)nrComponents);

			if (mipData != data) free(mipData);
			mipData = nextMip;
		}

		if (mipData != data) free(mipData);
	}
	else
	{
		mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			false,
			0,
			format,
			flags,
			bgfx::copy(data, width * height * nrComponents));
	}

	if (bgfx::isValid(mHandle))
		bgfx::setName(mHandle, texturename.data());
	else
	{
		const strg errmsg = "Failed loading texture: " + strg(texturename);
		throw BigError(errmsg);
	}

	mSize = vec2i(width, height);

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
}

void Texture::LoadGPUTexture(std::vector<uint8>& data, uint64 flags, strgv texturename)
{
	bx::DefaultAllocator allc;
#if __APPLE__
	bimg::ImageContainer* ic = bimg::imageParseKtx(&allc, data.data(), data.size(), nullptr);
#else
	bimg::ImageContainer* ic = bimg::imageParseDds(&allc, data.data(), data.size(), nullptr);
#endif
	if (!ic)
	{
		const strg errmsg = "Failed parsing GPU texture into container: " + strg(texturename);
		throw BigError(errmsg);
	}

	mHandle = bgfx::createTexture2D(
		static_cast<uint16>(ic->m_width),
		static_cast<uint16>(ic->m_height),
		(ic->m_numMips > 0),
		ic->m_numLayers,
		(bgfx::TextureFormat::Enum)ic->m_format,
		flags,
		bgfx::copy(ic->m_data, ic->m_size));

	if (bgfx::isValid(mHandle))
		bgfx::setName(mHandle, texturename.data());
	else
	{
		const strg errmsg = "Failed loading GPU texture: " + strg(texturename);
		throw BigError(errmsg);
	}

	mSize = vec2i(ic->m_width, ic->m_height);
	bimg::imageFree(ic);
}
