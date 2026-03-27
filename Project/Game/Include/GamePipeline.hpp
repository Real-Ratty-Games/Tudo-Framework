/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GAMEPIPELINE_HPP_
#define GAMEPIPELINE_HPP_
#include <SystemTypes.hpp>
#include <Memory.hpp>
#include <AssetLoader.hpp>
#include <DrawPipeline.hpp>
#include <GraphicsDevice.hpp>
#include <SpriteRenderer.hpp>
#include <DrawSurface2D.hpp>
#include <Shader.hpp>

using namespace Tudo;

namespace MyGame
{
	class GamePipeline : public DrawPipeline
	{
	public:
		GamePipeline(GraphicsDevice& gdevice, AssetLoader& assetloader, vec2 resolution);
		void Draw();
		void OnResize(vec2 size);

	private:
		void LoadShaders(AssetLoader& assetloader);

	private:
		Viewport2D					mCamera;

		SafePtr<DrawSurface2D>		mBackBufferSurface;

		SafePtr<SpriteRenderer>		mSpriteRenderer;

		SafePtr<Shader>				mSprite2DShader;
		SafePtr<Shader>				mSprite2DIShader;
		SafePtr<Shader>				mSprite2DAtlasShader;
		SafePtr<Shader>				mSprite2DAtlasIShader;
		SafePtr<Shader>				mColorQuadShader;
	};
}
#endif