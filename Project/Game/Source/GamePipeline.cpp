/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/GamePipeline.hpp"

using namespace MyGame;

GamePipeline::GamePipeline(GraphicsDevice& gdevice, AssetLoader& assetloader, vec2 resolution) : DrawPipeline(gdevice)
{
	// create sprite renderer
	mSpriteRenderer = new SpriteRenderer(gdevice, *this);

	// create unlit renderer
	mUnlitRenderer = new UnlitModelRenderer(gdevice, *this);

	// create color renderer
	mColorModelRenderer = new ColorModelRenderer(gdevice, *this);

	// create billboard renderer
	mBillboardRenderer = new BillboardRenderer(*pGDevice, *this);

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(gdevice, 0, resolution, nullptr);

	mCamera.Size = resolution;

	// load shaders
	LoadShaders(assetloader);



	_model = new Model3D(*pGDevice);
	assetloader.LoadModelFromFile(*_model, "lara.m3d");

	_tex = new Texture(gdevice);
	assetloader.LoadTextureFromFile(*_tex, "lara.png", BGFX_SAMPLER_MIN_POINT |
		BGFX_SAMPLER_MAG_POINT |
		BGFX_SAMPLER_U_CLAMP |
		BGFX_SAMPLER_V_CLAMP, "Font", false, true);

	mUnlitRenderer->SetTexture(_tex.Get());

	_3dsurface = new DrawSurface3D(*pGDevice, 1, resolution, nullptr, false);
	_sprite3d = new Sprite(*_3dsurface->GetTexture());

	_vp3d.Eye.Z = -5;
	_vp3d.CreateView();



	std::vector<ModelInstanceTransform> ttrs(255);

	for (int i = 0; i < 255; i++)
	{
		mat4 mdl = mat4::Identity();
		mdl = Math::Translate(mdl, vec3(i, 0, i), false);

		ModelInstanceTransform t;
		t.ModelMatrix = mdl;
		t.IData = Color(1, 0, 0, 1 - (i*.01f)).ToVec();
		ttrs[i] = t;
	}

	PrepareModelInstancing(*_model, _midata, ttrs);


	mColorModelRenderer->SetColor(0xffffffff);




	_ftex = new Texture(gdevice);

	assetloader.LoadTextureFromFile(*_ftex, "Font.png", BGFX_SAMPLER_MIN_POINT |
		BGFX_SAMPLER_MAG_POINT |
		BGFX_SAMPLER_U_CLAMP |
		BGFX_SAMPLER_V_CLAMP, "Font", false, false);

	_spriteFont = new Sprite(*_ftex);

	_ffont.pSprite = _spriteFont.Get();
	_ffont.GlyphSize = vec2(18, 30);
	_ffont.Glyphs =
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
		"ÄäÖöÜü"
		"0123456789"
		",;.:_-!?\"§$%&/()=*+~'#|<>²³{[]}\\";

	Transform2D transf;
	transf.Location = vec2(100, 100);
	mSpriteRenderer->PrepareSpriteFontText(_ffont, transf, _SimpleTextData, "Hello, Tudo!\nThis is 3D!!");

}

void GamePipeline::Draw()
{
	PrepareDrawModel(*_3dsurface, _vp3d);

		// SetActiveShader(mUnlitMeshIShader.Get());
		// mUnlitRenderer->DrawModelInstanced(_midata);

		SetActiveShader(mColorMeshIShader.Get());
		mColorModelRenderer->DrawModelInstanced(_midata);

	PrepareDrawSprite(*mBackBufferSurface, mCamera);

		SetActiveShader(mSprite2DShader.Get());

		Transform2D transf;
		mSpriteRenderer->DrawSprite(*_sprite3d, transf);

		SetActiveShader(mSprite2DAtlasIShader.Get());
		mSpriteRenderer->DrawSpriteFontText(_ffont, _SimpleTextData);
}

void GamePipeline::OnResize(vec2 size)
{
	mBackBufferSurface->OnResize(size);
	_3dsurface->OnResize(size);
}

void GamePipeline::LoadShaders(AssetLoader& assetloader)
{
	// default sprite shader
	mSprite2DShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DShader, "Shaders", "Sprite2D");

	// default sprite instancing shader
	mSprite2DIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DIShader, "Shaders", "Sprite2DI");

	// default sprite atlas shader
	mSprite2DAtlasShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasShader, "Shaders", "Sprite2DAtlas");

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasIShader, "Shaders", "Sprite2DAtlasI");

	// default color quad shader
	mColorQuadShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorQuadShader, "Shaders", "ColorQuad");

	// default unlit mesh shader
	mUnlitMeshShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mUnlitMeshShader, "Shaders", "UnlitMesh");

	// default unlit mesh instancing shader
	mUnlitMeshIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mUnlitMeshIShader, "Shaders", "UnlitMeshI");

	// default color mesh shader
	mColorMeshShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorMeshShader, "Shaders", "ColorMesh");

	// default color mesh instancing shader
	mColorMeshIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorMeshIShader, "Shaders", "ColorMeshI");

	// default billboard shader
	mBillboardShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mBillboardShader, "Shaders", "Billboard");

	pGDevice->InitShaderUniform("s_texColor",	bgfx::UniformType::Sampler);
	pGDevice->InitShaderUniform("u_color",		bgfx::UniformType::Vec4);
	pGDevice->InitShaderUniform("u_atlasInfo",	bgfx::UniformType::Vec4, 2);
	pGDevice->InitShaderUniform("u_scale",		bgfx::UniformType::Vec4);
}
