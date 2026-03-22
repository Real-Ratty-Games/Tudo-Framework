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
	mSpriteRenderer = new SpriteRenderer(pGDevice, this);

	// create unlit renderer
	mUnlitRenderer = new UnlitModelRenderer(pGDevice, this);

	// create color renderer
	mColorModelRenderer = new ColorModelRenderer(pGDevice, this);
	mColorModelRenderer->SetColor(0xffffffff);

	// create billboard renderer
	mBillboardRenderer = new BillboardRenderer(pGDevice, this);

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
	mSpriteRenderer->PrepareSpriteFontText(_ffont, transf, _SimpleTextData, "Look at this cool box!\nPretty neat, right?");



	_model = new Model3D(pGDevice);
	assetloader->LoadModelFromFile(*_model, "box.m3d");

	_3dsurface = new DrawSurface3D(pGDevice, 1, resolution, nullptr, false);
	_sprite3d = new Sprite(_3dsurface->GetTexture());

	_vp3d.Eye.Z = -5;
	_vp3d.CreateView();
}

void GamePipeline::Draw()
{
	PrepareDrawModel(_3dsurface.Get(), _vp3d);
	SetActiveDrawSurface(_3dsurface.Get());

	SetActiveShader(mColorMeshShader.Get());
	mColorModelRenderer->DrawModel(*_model);

	SetActiveShader(mBillboardShader.Get());
	mBillboardRenderer->Draw(_ftex.Get(), vec2(1.0f), 0xffffffff, false);


	// need a way to set transform for 3d stuff...



	SetActiveDrawSurface(mBackBufferSurface.Get());
	mSpriteRenderer->BeginDrawSprite(mCamera);
	// Render here...

	SetActiveShader(mSprite2DShader.Get());

	Transform2D transf;
	mSpriteRenderer->DrawSprite(_sprite3d.Get(), transf);

	SetActiveShader(mSprite2DAtlasIShader.Get());
	mSpriteRenderer->DrawSpriteFontText(_ffont, _SimpleTextData);

	mSpriteRenderer->EndDrawSprite();
}

void GamePipeline::OnResize(const vec2& size)
{
	mBackBufferSurface->OnResize(size);
	_3dsurface->OnResize(size);
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

	// unlit mesh shader
	mUnlitMeshShader = new Shader(pGDevice);
	assetloader->LoadShader(*mUnlitMeshShader, "Shaders", "UnlitMesh");
	mUnlitMeshShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);

	// color mesh shader
	mColorMeshShader = new Shader(pGDevice);
	assetloader->LoadShader(*mColorMeshShader, "Shaders", "ColorMesh");
	mColorMeshShader->InitUniform("color", bgfx::UniformType::Vec4);

	// billboard shader
	mBillboardShader = new Shader(pGDevice);
	assetloader->LoadShader(*mBillboardShader, "Shaders", "Billboard");
	mBillboardShader->InitUniform("scale", bgfx::UniformType::Vec4);
	mBillboardShader->InitUniform("color", bgfx::UniformType::Vec4);
	mBillboardShader->InitUniform("s_texColor", bgfx::UniformType::Sampler);
}
