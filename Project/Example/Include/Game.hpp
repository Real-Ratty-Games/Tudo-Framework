/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GAME_HPP_
#define GAME_HPP_
#include <SystemTypes.hpp>
#include <Memory.hpp>
#include <Program.hpp>
#include <Clock.hpp>
#include <Sound.hpp>
#include <GraphicsDevice.hpp>
#include <AssetLoader.hpp>
#include <WindowCursor.hpp>
#include <Input.hpp>

using namespace Tudo;

namespace MyGame
{
	class GameWindow;
	class GamePipeline;

	class GameProgram : public Program
	{
	public:
		void OnResize(vec2i& size);

	protected:
		bool Initialize();
		void Tick();
		void Draw();
		void Cleanup();

	private:
		SafePtr<GameWindow>		mWindow;
		WindowCursor			mCursor;
		Input					mInput;
		Clock					mClock = Clock(60.0f);

		SafePtr<SoundManager>	mSound;

		SafePtr<AssetLoader>	mAssetLoader;

		SafePtr<GraphicsDevice>	mGDevice;
		SafePtr<GamePipeline>	mPipeline;
	};
}
#endif