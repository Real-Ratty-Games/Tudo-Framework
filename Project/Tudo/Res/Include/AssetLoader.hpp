/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ASSETLOADER_HPP_
#define ASSETLOADER_HPP_
#include "SystemTypes.hpp"
#include "Globals.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"
#include <vector>

namespace Tudo
{
	class	GraphicsDevice;
	class	WindowCursor;
	class	Texture;
	class	Model3D;
	class	Shader;
	struct	SoundWav;
	struct	SoundWavStream;

	class AssetLoader : public DrawObject
	{
	public:
		AssetLoader(GraphicsDevice& gdevice, strgv rootFolder);

		// For uncompressed textures
		void LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen);
		void LoadTextureFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen);

		// For GPU friendly textures
		// DDS on Windows and KTX on macOS
		void LoadTextureGPUFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename);
		void LoadTextureGPUFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename);

		void LoadModelFromFile(Model3D& model, strgv filename);
		void LoadModelFromMemory(Model3D& model, const std::vector<uint8>& data);

		void LoadSoundFromFile(SoundWav& obj, strgv filepath);
		void StreamSoundFromFile(SoundWavStream& obj, strgv filepath);
		void LoadSoundFromMemory(SoundWav& obj, const std::vector<uint8>& data);

		void LoadShader(Shader& shader, strgv rootFolder, strgv shadername);

		/// file must be bmp
		void LoadHardwareCursorImage(WindowCursor& cursor, strgv filename);

	private:
		void LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents,
			strgv texturename, int width, int height, bool mipgen);
		void LoadGPUTexture(Texture& texture, const std::vector<uint8>& data, uint64 flags, strgv texturename);

		void CreateMesh(Mesh3D& modelMesh, MeshData& mdata);

		const bgfx::Memory* ShaderGetMemory(std::ifstream& file);

	private:
		strg mRootDir;
	};
}
#endif