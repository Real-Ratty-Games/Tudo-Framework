/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SHADER_HPP_
#define SHADER_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"

namespace Tudo
{
	class GraphicsDevice;

	class Shader : public DrawObject
	{
	public:
		Shader(GraphicsDevice& gdevice);
		~Shader();

		void Submit(uint16 viewID, uint8 flags, bool depth);
		bgfx::ProgramHandle& GetProgramHandle();

		friend class AssetLoader;

	private:
		bgfx::ProgramHandle mHandle;
	};
}
#endif