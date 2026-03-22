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
#include <UnlitModelRenderer.hpp>
#include <ColorModelRenderer.hpp>
#include <BillboardRenderer.hpp>
#include <DrawSurface2D.hpp>
#include <Shader.hpp>

#include <Texture.hpp>
#include <Sprite.hpp>

#include <Model3D.hpp>
#include <DrawSurface3D.hpp>
#include <Viewport3D.hpp>

using namespace Tudo;

namespace MyGame
{
	class GamePipeline : public DrawPipeline
	{
	public:
		GamePipeline(GraphicsDevice* gdevice, AssetLoader* assetloader, const vec2& resolution);
		void Draw();
		void OnResize(const vec2& size);

		Viewport3D _vp3d;

	private:
		void LoadShaders(AssetLoader* assetloader);

	private:
		Viewport2D					mCamera;

		SafePtr<DrawSurface2D>		mBackBufferSurface;

		SafePtr<SpriteRenderer>		mSpriteRenderer;
		SafePtr<UnlitModelRenderer>	mUnlitRenderer;
		SafePtr<ColorModelRenderer> mColorModelRenderer;
		SafePtr<BillboardRenderer>	mBillboardRenderer;

		SafePtr<Shader>				mSprite2DShader;
		SafePtr<Shader>				mSprite2DIShader;
		SafePtr<Shader>				mSprite2DAtlasShader;
		SafePtr<Shader>				mSprite2DAtlasIShader;

		SafePtr<Shader>				mUnlitMeshShader;
		SafePtr<Shader>				mColorMeshShader;
		SafePtr<Shader>				mBillboardShader;

		SafePtr< Texture> _ftex;
		SafePtr< Sprite> _spriteFont;
		SpriteFont _ffont;
		SpriteInstanceData _SimpleTextData;

		SafePtr<Model3D> _model;
		SafePtr<DrawSurface3D> _3dsurface;
		SafePtr< Sprite> _sprite3d;
	};
}
#endif