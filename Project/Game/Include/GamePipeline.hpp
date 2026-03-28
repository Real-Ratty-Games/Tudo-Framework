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
#include <DrawSurface2D.hpp>
#include <Shader.hpp>

#include <SpriteRenderer.hpp>
#include <UnlitModelRenderer.hpp>
#include <ColorModelRenderer.hpp>
#include <BillboardRenderer.hpp>

#include <Sprite.hpp>
#include <Viewport3D.hpp>
#include <Model3D.hpp>
#include <DrawSurface3D.hpp>
#include <Texture.hpp>

using namespace Tudo;

namespace MyGame
{
	class GamePipeline : public DrawPipeline
	{
	public:
		GamePipeline(GraphicsDevice& gdevice, AssetLoader& assetloader, vec2 resolution);
		void OnResize(vec2 size);

	protected:
		void Draw();

	private:
		void LoadShaders(AssetLoader& assetloader);

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
		SafePtr<Shader>				mColorQuadShader;

		SafePtr<Shader>				mUnlitMeshShader;
		SafePtr<Shader>				mUnlitMeshIShader;
		SafePtr<Shader>				mColorMeshShader;
		SafePtr<Shader>				mColorMeshIShader;
		SafePtr<Shader>				mBillboardShader;

		Viewport3D _vp3d;
		SafePtr<Texture> _tex;
		SafePtr<Model3D> _model;
		SafePtr<DrawSurface3D> _3dsurface;
		SafePtr<Sprite> _sprite3d;
		ModelInstanceData _midata;


		SafePtr< Texture> _ftex;
		SafePtr< Sprite> _spriteFont;
		SpriteFont _ffont;
		SpriteInstanceData _SimpleTextData;
	};
}
#endif