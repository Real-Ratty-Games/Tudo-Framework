/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include "../Include/GameWindow.hpp"
#include <FileSystem.hpp>
#include <Renderer.hpp>
#include <Input.hpp>

using namespace MyGame;

static WindowCursor* _SdlCursor;

void GameProgram::OnResize(vec2i& size)
{
	mBackBufferSurface->OnResize(size);
}

bool GameProgram::Initialize()
{
	// create window
	Window::Initialize();
	Window::ShowSplashScreen();

	mSound.Initialize();

	const strg cursorPath = FileSystem::GetResourcePath("Data/Cursor.bmp").string();
	_SdlCursor = Window::LoadHardwareCursorImage(cursorPath);
	Window::SetHardwareCursorImage(_SdlCursor);

	const vec2 resolution(1280, 720);

	mWindow = new GameWindow(this);
	mWindow->Create("My Game", (uint)resolution.X, (uint)resolution.Y, false);

	// init renderer
	DrawAPI dapi = DrawAPI::DIRECT3D11;
#if __APPLE__
	dapi = DrawAPI::METAL;
#endif

	if (!Renderer::Initialize(mWindow, dapi, true, MSAA::NONE))
		return false;

	// setup shaders
	const strg shaderPath = FileSystem::GetResourcePath("Data/Shaders").string();
	const strg shaderDevPath = FileSystem::GetResourcePath("Data/Development/Shaders").string();

	Shader::SetShaderDirectory(shaderPath);
	const strg result = Shader::CompileAllShaders(shaderDevPath);

	const strg shdErrPath = FileSystem::GetResourcePath("ShaderErr.log").string();
	FileSystem::WriteTextFile(shdErrPath, result);

	LoadShaders();

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(0, resolution, mWindow->GetNativePtr());

	mCamera.Size = mWindow->GetSize();

	Window::DestroySplashScreen();
	mWindow->Show();
	return true;
}

void GameProgram::Tick()
{
	mWindow->PollEvent();

	if (Keyboard::KeyPressed(KeyboardKey::N1))
		mWindow->SwitchFullscreen();

	if (Keyboard::KeyPressed(KeyboardKey::ESCAPE))
		Quit();

	mClock.Tick();
	while (mClock.Wait())
	{
		// Logic here...
	}
}

void GameProgram::Draw()
{
	Renderer::BeginDraw();
	if (!mWindow->IsIconified())
	{
		Renderer::BeginDrawSprite(mBackBufferSurface, mCamera);
		// Render here...
		Renderer::EndDrawSprite();
	}
	Renderer::EndDraw();
}

void GameProgram::Cleanup()
{
	if (mBackBufferSurface != nullptr)
	{
		mBackBufferSurface->Release();
		delete mBackBufferSurface;
		mBackBufferSurface = nullptr;
	}

	FreeShaders();
	Renderer::Release();

	mSound.Release();

	Window::SetHardwareCursorImage(nullptr);
	Window::DestroyCursor(_SdlCursor);

	if (mWindow != nullptr)
	{
		delete mWindow;
		mWindow = nullptr;
	}
	Window::Release();
}

void GameProgram::LoadShaders()
{
	// default sprite shader
	mSprite2DShader.Initialize("Sprite2D");
	mSprite2DShader.InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DShader.InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite instancing shader
	mSprite2DIShader.Initialize("Sprite2DI");
	mSprite2DIShader.InitUniform("s_texColor", bgfx::UniformType::Sampler);

	// default sprite atlas shader
	mSprite2DAtlasShader.Initialize("Sprite2DAtlas");
	mSprite2DAtlasShader.InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasShader.InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
	mSprite2DAtlasShader.InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader.Initialize("Sprite2DAtlasI");
	mSprite2DAtlasIShader.InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasIShader.InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
}

void GameProgram::FreeShaders()
{
	mSprite2DShader.Release();
	mSprite2DIShader.Release();
	mSprite2DAtlasShader.Release();
	mSprite2DAtlasIShader.Release();
}
