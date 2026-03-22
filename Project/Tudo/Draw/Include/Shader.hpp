/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SHADER_HPP_
#define SHADER_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"
#include <unordered_map>

namespace Tudo
{
	class GraphicsDevice;
	class Texture;

	class Shader : public DrawObject
	{
	public:
		Shader(GraphicsDevice* gdevice);
		~Shader();

		void Submit(uint16 viewID, uint8 flags, const bool depth);

		void InitUniform(strgv name, bgfx::UniformType::Enum type, uint16 nmb = 1);
		void SetUniform(strgv name, const void* vl, uint16 nmb = 1);
		void SetTexture(uint8 stage, strgv name, Texture& texture);

		bgfx::UniformHandle* GetUniform(strgv name);
		bgfx::ProgramHandle& GetProgramHandle();

		friend class AssetLoader;

	private:
		bgfx::ProgramHandle								mHandle;
		std::unordered_map<strg, bgfx::UniformHandle>	mUniforms;
	};
}
#endif