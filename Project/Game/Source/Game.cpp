/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include "../Include/GameWindow.hpp"
#include "../Include/GamePipeline.hpp"
#include <FileSystem.hpp>

using namespace MyGame;

void GameProgram::OnResize(vec2i& size)
{
	mGDevice->OnResize(size, true);
	mPipeline->OnResize(size);
}

bool GameProgram::Initialize()
{
	// create window
	Window::Initialize();
	Window::ShowSplashScreen();

	mSound = new SoundManager();

	const vec2 resolution(1280, 720);

	mWindow = new GameWindow(this);
	mWindow->Create("My Game", (uint)resolution.X, (uint)resolution.Y, false);

	// init renderer
	DrawAPI dapi = DrawAPI::DIRECT3D11;
#if __APPLE__
	dapi = DrawAPI::METAL;
#endif

	mGDevice = new GraphicsDevice(mWindow.Get(), dapi, true);

	mAssetLoader = new AssetLoader(mGDevice.Get(), "Data");

	mPipeline = new GamePipeline(mGDevice.Get(), mAssetLoader.Get(), resolution);

	// set hw cursor
	mAssetLoader->LoadHardwareCursorImage(mCursor, "Cursor.bmp");
	mWindow->SetHardwareCursorImage(&mCursor);

	Window::DestroySplashScreen();
	mWindow->Show();
	return true;
}

void GameProgram::Tick()
{
	mWindow->PollEvent();

	if (mInput.KeyboardKeyPressed(KeyboardKey::N1))
		mWindow->SwitchFullscreen();

	if (mInput.KeyboardKeyPressed(KeyboardKey::ESCAPE))
		Quit();

	mClock.Tick();
	while (mClock.Wait())
	{
		// Logic here...
	}
}

void GameProgram::Draw()
{
	mGDevice->BeginDraw();
	if (!mWindow->IsIconified())
		mPipeline->Draw();
	mGDevice->EndDraw();
}

void GameProgram::Cleanup()
{
	Window::SetHardwareCursorImage(nullptr);
	Window::Release();
}
