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

using namespace GameEngine;

/// Base shader directory
static strg _ShaderDir;

static const bgfx::Memory*	Shader_GetMemory(bx::FileReader& filereader);
static strg					Shader_CompileShader(strgv args);

void Shader::SetShaderDirectory(strgv dir)
{
	_ShaderDir = dir;
}

strg Shader::CompileAllShaders(strgv dir)
{
	strg result;

	const std::vector<strg> shaders = FileSystem::SubDirectories(dir);
	const strg ishaders = " -i " + strg(dir) + "/Shared";

	for (auto& it : shaders)
	{
		std::filesystem::path path(it);
		const strg name = path.filename().string();
		if (name == "Shared") continue;

		const strg inpath = strg(dir) + "/" + name + "/" + name;

#if _WIN32
		const strg outpathD3D = _ShaderDir + "/D3D/" + name;
		const strg outpathVK = _ShaderDir + "/SPIRV/" + name;

		const strg arg0 = "-f " + inpath + ".vs -o " + outpathD3D + ".vsb --type v --platform windows -p s_5_0 -O 3" + ishaders;
		result += Shader_CompileShader(arg0) + "\n";

		const strg arg1 = "-f " + inpath + ".fs -o " + outpathD3D + ".fsb --type f --platform windows -p s_5_0 -O 3" + ishaders;
		result += Shader_CompileShader(arg1) + "\n";

		const strg arg2 = "-f " + inpath + ".vs -o " + outpathVK + ".vsb --type v --platform windows -p spirv" + ishaders;
		result += Shader_CompileShader(arg2) + "\n";

		const strg arg3 = "-f " + inpath + ".fs -o " + outpathVK + ".fsb --type f --platform windows -p spirv" + ishaders;
		result += Shader_CompileShader(arg3) + "\n";

#elif __APPLE__
		const strg outpathMT = _ShaderDir + "/METAL/" + name;

		const strg arg4 = "-f " + inpath + ".vs -o " + outpathMT + ".vsb --type v --platform osx -p metal" + ishaders;
		result += Shader_CompileShader(arg4) + "\n";

		const strg arg5 = "-f " + inpath + ".fs -o " + outpathMT + ".fsb --type f --platform osx -p metal" + ishaders;
		result += Shader_CompileShader(arg5) + "\n";
#endif
	}

	return result;
}

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

void Shader::Initialize(strgv shadername)
{
	bgfx::RendererType::Enum type = bgfx::getRendererType();
	strg typenm = "D3D";
	if (type == bgfx::RendererType::Vulkan)
		typenm = "SPIRV";
	else if (type == bgfx::RendererType::Metal)
		typenm = "METAL";

	strg path(_ShaderDir);
	path += "/" + typenm + "/" + strg(shadername);

	strg vpath = path + ".vsb";
	strg fpath = path + ".fsb";

	bx::FileReader vFileReader;
	bx::Error err;
	if (!vFileReader.open(vpath.c_str(), &err))
	{
		const strg errmsg = "Failed loading vertex shader of: " + strg(shadername);
		throw BigError(errmsg);
	}

	bx::FileReader fFileReader;
	if (!fFileReader.open(fpath.c_str(), &err))
	{
		const strg errmsg = "Failed loading fragment shader of: " + strg(shadername);
		throw BigError(errmsg);
	}

	if (err.isOk())
	{
		bgfx::ShaderHandle vsh = bgfx::createShader(Shader_GetMemory(vFileReader));
		bgfx::setName(vsh, strg(strg(shadername) + "_vs").c_str());

		bgfx::ShaderHandle fsh = bgfx::createShader(Shader_GetMemory(fFileReader));
		bgfx::setName(vsh, strg(strg(shadername) + "_fs").c_str());

		mHandle = bgfx::createProgram(vsh, fsh, true);
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

/*======================================================
======================================================*/

const bgfx::Memory* Shader_GetMemory(bx::FileReader& filereader)
{
	bx::Error err;
	bx::seek(&filereader, 0, bx::Whence::End);
	uint size = uint(bx::getSize(&filereader));
	const bgfx::Memory* mem = bgfx::alloc(size + 1);
	bx::seek(&filereader, 0, bx::Whence::Begin);
	bx::read(&filereader, mem->data, size, &err);
	bx::close(&filereader);
	mem->data[size] = '\0';
	return mem;
}

strg Shader_CompileShader(strgv args)
{
	std::ostringstream command;

#if _WIN32
	command << "Shaderc.exe " << args;
#elif __APPLE__
	const strg exepath = FileSystem::GetResourcePath("shadercRelease").string() + " ";
	command << exepath << args;
#endif

#if _WIN32
	FILE* pipe = _popen(command.str().c_str(), "r");
#elif __APPLE__
	FILE* pipe = popen(command.str().c_str(), "r");
#endif
	if (!pipe)
	{
		const strg errmsg = "Failed opening Shaderc: " + command.str();
		throw BigError(errmsg);
	}

	std::ostringstream output;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		output << buffer;

#if _WIN32
	if (_pclose(pipe) == -1)
#elif __APPLE__
	if (pclose(pipe) == -1)
#endif
	{
		const strg errmsg = "Failed closing Shaderc: " + command.str();
		throw BigError(errmsg);
	}

	return output.str();
}
