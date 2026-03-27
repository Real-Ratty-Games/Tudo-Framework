/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "AssetLoader.hpp"
#include "Memory.hpp"
#include "FileSystem.hpp"
#include "Math.hpp"
#include "BigError.hpp"
#include "GraphicsDevice.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Model3D.hpp"
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include "Sound.hpp"
#include "WindowCursor.hpp"
#include "Window.hpp"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

using namespace Tudo;

AssetLoader::AssetLoader(GraphicsDevice& gdevice, strgv rootFolder) : DrawObject(gdevice)
{
	mRootDir = FileSystem::GetResourcePath(rootFolder).string();
}

void AssetLoader::LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load(realpath.data(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from file: " + strg(filename);
		throw BigError(errmsg);
	}

	LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

void AssetLoader::LoadTextureFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load_from_memory(memData.data(), (uint)memData.size(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from memory: " + strg(texturename);
		throw BigError(errmsg);
	}

	LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

void AssetLoader::LoadTextureGPUFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	std::ifstream file(realpath.data(), std::ios::binary);
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

	LoadGPUTexture(texture, data, flags, texturename);
}

void AssetLoader::LoadTextureGPUFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename)
{
	LoadGPUTexture(texture, memData, flags, texturename);
}

void AssetLoader::LoadModelFromFile(Model3D& model, strgv filename)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	std::ifstream file(realpath.data(), std::ios::binary);
	if (!file)
	{
		const strg errmsg = "Model file could not be found: " + strg(filename);
		throw BigError(errmsg);
	}

	file.seekg(0, std::ios::beg);

	M3DCHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(M3DCHeader));

	if (header.Magic != M3DC_MAGIC)
	{
		const strg errmsg = "Model file format invalid: " + strg(filename);
		throw BigError(errmsg);
	}

	if (header.Version != M3DC_VERSION)
	{
		const strg errmsg = "Model file format uses older version: " + strg(filename);
		throw BigError(errmsg);
	}

	model.mMeshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;
		file.read(reinterpret_cast<char*>(&vsize), sizeof(uint64));
		file.read(reinterpret_cast<char*>(&isize), sizeof(uint64));

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		file.read(reinterpret_cast<char*>(mdata.Vertices.data()), vsize * sizeof(MeshVertex));
		file.read(reinterpret_cast<char*>(mdata.Indices.data()), isize * sizeof(uint16));

		CreateMesh(model.mMeshes[i], mdata);
	}
}

void AssetLoader::LoadModelFromMemory(Model3D& model, const std::vector<uint8>& data)
{
	if (data.empty())
		throw BigError("Data is empty!");

	const uint8* rdat = data.data();
	uint64 offset = 0;

	M3DCHeader header;
	Memory::Copy(&header, rdat + offset, sizeof(M3DCHeader));
	offset += sizeof(M3DCHeader);

	if (header.Magic != M3DC_MAGIC)
		throw BigError("Model file format invalid");

	if (header.Version != M3DC_VERSION)
		throw BigError("Model file format uses older version");

	model.mMeshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;

		Memory::Copy(&vsize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		Memory::Copy(&isize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		const uint64 vtsize = vsize * sizeof(MeshVertex);
		Memory::Copy(mdata.Vertices.data(), rdat + offset, vtsize);
		offset += vtsize;

		const uint64 itsize = isize * sizeof(uint16);
		Memory::Copy(mdata.Indices.data(), rdat + offset, itsize);
		offset += itsize;

		CreateMesh(model.mMeshes[i], mdata);
	}
}

void AssetLoader::LoadSoundFromFile(SoundWav& obj, strgv filepath)
{
	const strg realpath = mRootDir + "/" + strg(filepath);
	if (obj.Handle.load(realpath.data()) != SoLoud::SO_NO_ERROR)
	{
		const strg errmsg = "Failed loading sound file: " + strg(filepath);
		throw BigError(errmsg);
	}
}

void AssetLoader::StreamSoundFromFile(SoundWavStream& obj, strgv filepath)
{
	const strg realpath = mRootDir + "/" + strg(filepath);
	if (obj.Handle.load(realpath.data()) != SoLoud::SO_NO_ERROR)
	{
		const strg errmsg = "Failed streaming sound file: " + strg(filepath);
		throw BigError(errmsg);
	}
}

void AssetLoader::LoadSoundFromMemory(SoundWav& obj, const std::vector<uint8>& data)
{
	if (obj.Handle.loadMem(data.data(), static_cast<uint>(data.size()), false, false) != SoLoud::SO_NO_ERROR)
		throw BigError("Failed loading sound file from memory!");
}

void AssetLoader::LoadShader(Shader& shader, strgv rootFolder, strgv shadername)
{
	const strg realpath = mRootDir + "/" + strg(rootFolder);

	bgfx::RendererType::Enum type = bgfx::getRendererType();
	strg typenm = "D3D";
	if (type == bgfx::RendererType::Vulkan)
		typenm = "SPIRV";
	else if (type == bgfx::RendererType::Metal)
		typenm = "METAL";

	strg path(realpath);
	path += "/" + typenm + "/" + strg(shadername);

	strg vpath = path + ".vsb";
	strg fpath = path + ".fsb";

	std::ifstream vfile(vpath.data(), std::ios::binary);
	if (!vfile)
	{
		const strg errmsg = "Failed loading vertex shader of: " + strg(shadername);
		throw BigError(errmsg);
	}

	std::ifstream ffile(fpath.data(), std::ios::binary);
	if (!ffile)
	{
		const strg errmsg = "Failed loading fragment shader of: " + strg(shadername);
		throw BigError(errmsg);
	}

	bgfx::ShaderHandle vsh = bgfx::createShader(ShaderGetMemory(vfile));
	bgfx::setName(vsh, strg(strg(shadername) + "_vs").c_str());

	bgfx::ShaderHandle fsh = bgfx::createShader(ShaderGetMemory(ffile));
	bgfx::setName(vsh, strg(strg(shadername) + "_fs").c_str());

	shader.mHandle = bgfx::createProgram(vsh, fsh, true);
}

void AssetLoader::LoadHardwareCursorImage(WindowCursor& cursor, strgv filename)
{
	const strg realpath = mRootDir + "/" + strg(filename);
	SDL_Surface* cursorSurface = SDL_LoadBMP(realpath.data());
	if (cursorSurface != nullptr)
	{
		cursor.mCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
		SDL_DestroySurface(cursorSurface);
	}
	else throw BigError("Image file not found: " + strg(filename));
}

void AssetLoader::LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents,
	strgv texturename, int width, int height, bool mipgen)
{
	bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
	if (nrComponents == 3)
		format = bgfx::TextureFormat::RGB8;
	else if (nrComponents == 4)
		format = bgfx::TextureFormat::RGBA8;

	if (mipgen)
	{
		int nmips = Math::Maxi(width, height);
		nmips = 1 + static_cast<int>(std::floor(std::log2(nmips)));

		texture.mHandle = bgfx::createTexture2D(
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
				texture.mHandle,
				0,
				i,
				0, 0,
				static_cast<uint16>(mipWidth),
				static_cast<uint16>(mipHeight),
				bgfx::copy(mipData, mipWidth * mipHeight * nrComponents));

			mipWidth = Math::Maxi(1, mipWidth / 2);
			mipHeight = Math::Maxi(1, mipHeight / 2);
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
		texture.mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			false,
			0,
			format,
			flags,
			bgfx::copy(data, width * height * nrComponents));
	}

	if (bgfx::isValid(texture.mHandle))
		bgfx::setName(texture.mHandle, texturename.data());
	else
	{
		const strg errmsg = "Failed loading texture: " + strg(texturename);
		throw BigError(errmsg);
	}

	texture.mSize = vec2i(width, height);

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
}

void AssetLoader::LoadGPUTexture(Texture& texture, const std::vector<uint8>& data, uint64 flags, strgv texturename)
{
	bx::DefaultAllocator allc;
#if __APPLE__
	bimg::ImageContainer* ic = bimg::imageParseKtx(&allc, data.data(), data.size(), nullptr);
#else
	bimg::ImageContainer* ic = bimg::imageParseDds(&allc, data.data(), (uint)data.size(), nullptr);
#endif
	if (!ic)
	{
		const strg errmsg = "Failed parsing GPU texture into container: " + strg(texturename);
		throw BigError(errmsg);
	}

	texture.mHandle = bgfx::createTexture2D(
		static_cast<uint16>(ic->m_width),
		static_cast<uint16>(ic->m_height),
		(ic->m_numMips > 0),
		ic->m_numLayers,
		(bgfx::TextureFormat::Enum)ic->m_format,
		flags,
		bgfx::copy(ic->m_data, ic->m_size));

	if (bgfx::isValid(texture.mHandle))
		bgfx::setName(texture.mHandle, texturename.data());
	else
	{
		const strg errmsg = "Failed loading GPU texture: " + strg(texturename);
		throw BigError(errmsg);
	}

	texture.mSize = vec2i(ic->m_width, ic->m_height);
	bimg::imageFree(ic);
}

void AssetLoader::CreateMesh(Mesh3D& modelMesh, MeshData& mdata)
{
	modelMesh.VBH = pGDevice->CreateVertexBuffer(mdata.Vertices.data(), (uint)(mdata.VSize * sizeof(MeshVertex)), pGDevice->mMesh3DVBLayout);
	if (!bgfx::isValid(modelMesh.VBH))
		throw BigError("Vertex Buffer is invalid!");

	modelMesh.IBH = pGDevice->CreateIndexBuffer(mdata.Indices.data(), (uint)(mdata.ISize * sizeof(uint16)));
	if (!bgfx::isValid(modelMesh.IBH))
		throw BigError("Index Buffer is invalid!");
}

const bgfx::Memory* AssetLoader::ShaderGetMemory(std::ifstream& file)
{
	file.seekg(0, std::ios::end);
	const uint64 size = file.tellg();
	file.seekg(0, std::ios::beg);

	const bgfx::Memory* mem = bgfx::alloc((uint)size + 1);
	file.read(reinterpret_cast<char*>(mem->data), size);
	return mem;
}
