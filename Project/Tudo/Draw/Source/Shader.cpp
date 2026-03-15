/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Shader.hpp"
#include "FileSystem.hpp"
#include "BigError.hpp"
#include "Texture.hpp"
#include <vector>
#include <sstream>
#include <filesystem>
#include <bx/bx.h>
#include <bx/file.h>

using namespace Tudo;

Shader::Shader(Renderer* renderer) : DrawObject(renderer)
{
	mHandle = BGFX_INVALID_HANDLE;
}

Shader::~Shader()
{
	if (bgfx::isValid(mHandle))
	{
		for (auto& it : mUniforms)
		{
			if (bgfx::isValid(it.second))
				bgfx::destroy(it.second);
		}
		mUniforms.clear();

		bgfx::destroy(mHandle);
	}
}

void Shader::Submit(uint16 viewID, uint8 flags, const bool depth)
{
	bgfx::submit(viewID, mHandle, depth, flags);
}

void Shader::InitUniform(strgv name, bgfx::UniformType::Enum type, uint16 nmb)
{
	const char* nm = name.data();
	mUniforms[nm] = bgfx::createUniform(nm, type, nmb);
}

void Shader::SetUniform(strgv name, const void* vl, uint16 nmb)
{
	bgfx::setUniform(mUniforms[name.data()], vl, nmb);
}

void Shader::SetTexture(uint8 stage, strgv name, Texture& texture)
{
	bgfx::setTexture(stage, mUniforms[name.data()], texture.Handle());
}

bgfx::UniformHandle* Shader::GetUniform(strgv name)
{
	if (mUniforms.contains(name.data()))
		return &mUniforms[name.data()];
	else return nullptr;
}

bgfx::ProgramHandle& Shader::GetProgramHandle()
{
	return mHandle;
}
