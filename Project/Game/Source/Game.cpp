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

void GameProgram::OnResize(vec2i& size)
{
	mRenderer.OnResize(size, true);
	mBackBufferSurface->OnResize(size);
}

bool GameProgram::Initialize()
{
	// create window
	Window::Initialize();
	Window::ShowSplashScreen();

	mSound.Initialize();

	const vec2 resolution(1280, 720);

	mWindow = new GameWindow(this);
	mWindow->Create("My Game", (uint)resolution.X, (uint)resolution.Y, false);

	// init renderer
	DrawAPI dapi = DrawAPI::DIRECT3D11;
#if __APPLE__
	dapi = DrawAPI::METAL;
#endif

	if (!mRenderer.Initialize(mWindow.Get(), dapi, true))
		return false;

	mAssetLoader = new AssetLoader(&mRenderer, "Data");

	// set hw cursor
	mAssetLoader->LoadHardwareCursorImage(&mCursor, "Cursor.bmp");
	mWindow->SetHardwareCursorImage(&mCursor);

	// load shaders
	LoadShaders();

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(&mRenderer, 0, resolution, mWindow->GetNativePtr());

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
	mRenderer.BeginDraw();
	if (!mWindow->IsIconified())
	{
		mRenderer.BeginDrawSprite(mBackBufferSurface.Get(), mCamera);
		// Render here...
		mRenderer.EndDrawSprite();
	}
	mRenderer.EndDraw();
}

void GameProgram::Cleanup()
{
	Window::SetHardwareCursorImage(nullptr);
	Window::Release();
}

void GameProgram::LoadShaders()
{
	// default sprite shader
	mSprite2DShader = new Shader(&mRenderer);
	mAssetLoader->LoadShader(mSprite2DShader.Get(), "Shaders", "Sprite2D");
	mSprite2DShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite instancing shader
	mSprite2DIShader = new Shader(&mRenderer);
	mAssetLoader->LoadShader(mSprite2DIShader.Get(), "Shaders", "Sprite2DI");
	mSprite2DIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);

	// default sprite atlas shader
	mSprite2DAtlasShader = new Shader(&mRenderer);
	mAssetLoader->LoadShader(mSprite2DAtlasShader.Get(), "Shaders", "Sprite2DAtlas");
	mSprite2DAtlasShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
	mSprite2DAtlasShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader = new Shader(&mRenderer);
	mAssetLoader->LoadShader(mSprite2DAtlasIShader.Get(), "Shaders", "Sprite2DAtlasI");
	mSprite2DAtlasIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasIShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
}
