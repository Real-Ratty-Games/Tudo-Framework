/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/GamePipeline.hpp"

using namespace MyGame;

GamePipeline::GamePipeline(GraphicsDevice& gdevice, AssetLoader& assetloader, vec2 resolution) : DrawPipeline(gdevice)
{
	// create sprite renderer
	mSpriteRenderer = new SpriteRenderer(*pGDevice, *this);

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(*pGDevice, 0, resolution, nullptr);

	mCamera.Size = resolution;

	// load shaders
	LoadShaders(assetloader);
}

void GamePipeline::Draw()
{
	PrepareDrawSprite(*mBackBufferSurface, mCamera);

	SetActiveShader(mColorQuadShader.Get());

	Transform2D transf;
	mSpriteRenderer->DrawColorQuad(transf, 0, 128);
}

void GamePipeline::OnResize(vec2 size)
{
	mBackBufferSurface->OnResize(size);
}

void GamePipeline::LoadShaders(AssetLoader& assetloader)
{
	// default sprite shader
	mSprite2DShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DShader, "Shaders", "Sprite2D");
	mSprite2DShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite instancing shader
	mSprite2DIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DIShader, "Shaders", "Sprite2DI");
	mSprite2DIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);

	// default sprite atlas shader
	mSprite2DAtlasShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasShader, "Shaders", "Sprite2DAtlas");
	mSprite2DAtlasShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
	mSprite2DAtlasShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasIShader, "Shaders", "Sprite2DAtlasI");
	mSprite2DAtlasIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasIShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);

	// default color quad shader
	mColorQuadShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorQuadShader, "Shaders", "ColorQuad");
	mColorQuadShader->InitUniform("color", bgfx::UniformType::Vec4);
}
