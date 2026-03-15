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
#include <Renderer.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <Sprite.hpp>
#include <DrawSurface2D.hpp>
#include <DrawSurface3D.hpp>
#include <SpriteAnimation.hpp>
#include <AssetLoader.hpp>
#include <WindowCursor.hpp>

using namespace Tudo;

namespace MyGame
{
	class GameWindow;

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
		void LoadShaders();

	private:
		SafePtr<GameWindow>		mWindow;
		WindowCursor			mCursor;
		Clock					mClock = Clock(60.0f);

		SoundManager			mSound;

		SafePtr<AssetLoader>	mAssetLoader;

		Renderer				mRenderer;
		Viewport2D				mCamera;
		SafePtr<Shader>			mSprite2DShader;
		SafePtr<Shader>			mSprite2DIShader;
		SafePtr<Shader>			mSprite2DAtlasShader;
		SafePtr<Shader>			mSprite2DAtlasIShader;
		SafePtr<DrawSurface2D>	mBackBufferSurface;
	};
}
#endif