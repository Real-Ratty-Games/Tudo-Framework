/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/GamePipeline.hpp"

using namespace MyGame;

static float mod = 1;

GamePipeline::GamePipeline(GraphicsDevice* gdevice, AssetLoader* assetloader, const vec2& resolution) : DrawPipeline(gdevice)
{
	// create sprite renderer
	mSpriteRenderer = new SpriteRenderer(pGDevice);

	// create unlit renderer
	mUnlitRenderer = new UnlitRenderer(pGDevice);

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(pGDevice, 0, resolution, nullptr);

	mCamera.Size = resolution;

	// load shaders
	LoadShaders(assetloader);


	_ftex = new Texture(gdevice);

	assetloader->LoadTextureFromFile(*_ftex, "Font.png", BGFX_SAMPLER_MIN_POINT |
		BGFX_SAMPLER_MAG_POINT |
		BGFX_SAMPLER_U_CLAMP |
		BGFX_SAMPLER_V_CLAMP, "Font", false, false);

	_spriteFont = new Sprite(_ftex.Get());

	_ffont.pSprite = _spriteFont.Get();
	_ffont.GlyphSize = vec2(18, 30);
	_ffont.Glyphs =
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
		"ÄäÖöÜü"
		"0123456789"
		",;.:_-!?\"§$%&/()=*+~'#|<>²³{[]}\\";

	Transform2D transf;
	transf.Location = vec2(100, 100);
	transf.Scale = vec2(1);
	transf.Rotation = 0.0f;
	transf.ImageColor = Color(0xffffffff);
	mSpriteRenderer->PrepareSpriteFontText(_ffont, transf, _SimpleTextData, "Look at this cool box!\nPretty neat, right?");

}

void GamePipeline::Draw()
{
	mSpriteRenderer->BeginDrawSprite(mBackBufferSurface.Get(), mCamera);
	// Render here...

	mSpriteRenderer->SetActiveShader(mSprite2DAtlasIShader.Get());
	mSpriteRenderer->DrawSpriteFontText(_ffont, _SimpleTextData);

	mSpriteRenderer->EndDrawSprite();
}

void GamePipeline::OnResize(const vec2& size)
{
	mBackBufferSurface->OnResize(size);
}

void GamePipeline::LoadShaders(AssetLoader* assetloader)
{
	// default sprite shader
	mSprite2DShader = new Shader(pGDevice);
	assetloader->LoadShader(*mSprite2DShader, "Shaders", "Sprite2D");
	mSprite2DShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite instancing shader
	mSprite2DIShader = new Shader(pGDevice);
	assetloader->LoadShader(*mSprite2DIShader, "Shaders", "Sprite2DI");
	mSprite2DIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);

	// default sprite atlas shader
	mSprite2DAtlasShader = new Shader(pGDevice);
	assetloader->LoadShader(*mSprite2DAtlasShader, "Shaders", "Sprite2DAtlas");
	mSprite2DAtlasShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
	mSprite2DAtlasShader->InitUniform("color", bgfx::UniformType::Vec4);

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader = new Shader(pGDevice);
	assetloader->LoadShader(*mSprite2DAtlasIShader, "Shaders", "Sprite2DAtlasI");
	mSprite2DAtlasIShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
	mSprite2DAtlasIShader->InitUniform("atlasInfo", bgfx::UniformType::Vec4, 2);
}
