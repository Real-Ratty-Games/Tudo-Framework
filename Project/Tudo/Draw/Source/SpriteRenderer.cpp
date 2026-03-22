/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "SpriteRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawSurface2D.hpp"
#include "Sprite.hpp"
#include "SpriteAnimation.hpp"
#include "Shader.hpp"
#include "Projection.hpp"

using namespace Tudo;

SpriteRenderer::SpriteRenderer(GraphicsDevice* gdevice) : Renderer(gdevice)
{
	pQuad2DLastTex		= nullptr;

	const vec3 eye(0.0f, 0.0f, -5.0f);
	const vec3 target(0.0f, 0.0f, 0.0f);
	mQuad2DView = Math::LookAtLH(eye, target, vec3(.0f, .1f, .0f));
}

void SpriteRenderer::BeginDrawSprite(DrawSurface2D* surface, Viewport2D& viewport)
{
	pActiveDrawSurface = surface;
	pActiveDrawSurface->Clear();

	mat4 proj = Math::ProjectOrthoLH(viewport.Location.X, viewport.Size.X + viewport.Location.Y,
		viewport.Size.Y + viewport.Location.Y, viewport.Location.Y, 0.1f, 100.0f, 0.0f);

	bgfx::setViewTransform(pActiveDrawSurface->ViewID(), mQuad2DView.Ptr(), proj.Ptr());
	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());
}

void SpriteRenderer::EndDrawSprite()
{
	ClearActives();
	pQuad2DLastTex = nullptr;
}

void SpriteRenderer::DrawSprite(Sprite* sprite, Transform2D& transformation)
{
	DrawTexture(sprite->GetTexture(), sprite->RotationPivot, sprite->Size, transformation);
}

void SpriteRenderer::DrawSpriteAtlas(Sprite* sprite, TransformAtlas2D& transformation, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(transformation.Index.X, transformation.Index.Y, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pActiveShader->SetUniform("atlasInfo", atinf, 2);
	DrawTexture(sprite->GetTexture(), sprite->RotationPivot, subSize, transformation);
}

void SpriteRenderer::PrepareSpriteInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<Transform2D>& tdata)
{
	const int insCnt = (int)tdata.size();
	const uint16 insStride = 64 + sizeof(vec4);
	uint drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	idata.MissedAmount = insCnt - drawnIns;
	idata.pSprite = sprite;

	bgfx::allocInstanceDataBuffer(&idata.Buffer, drawnIns, insStride);

	uint8* data = idata.Buffer.data;

	for (uint i = 0; i < drawnIns; i++)
	{
		auto& transf = tdata[i];
		vec2 rscale = sprite->Size * transf.Scale;

		vec3 rotPiv = vec3(
			sprite->RotationPivot.X * rscale.X,
			sprite->RotationPivot.Y * rscale.Y,
			0.0f
		);

		mat4 mdl = mat4::Identity();
		mdl = Math::Translate(mdl, vec3(transf.Location.X, transf.Location.Y, 0.0f), false);					// move to world pos
		mdl = Math::Translate(mdl, rotPiv, false);																// mov pivot to origin
		mdl = Math::Rotate(mdl, vec3(0.0f, 0.0f, 1.0f), Math::ToRadians(transf.Rotation), false);				// rotate
		mdl = Math::Translate(mdl, -rotPiv, false);																// move pivot back
		mdl = Math::Scale(mdl, vec3(rscale.X, rscale.Y, 1.0f), false);											// scale


		const float* _mtx = mdl.Ptr();
		Memory::Copy(data, _mtx, insStride);
		data += 64;

		vec4 color = vec4(transf.ImageColor.R, transf.ImageColor.G, transf.ImageColor.B, transf.ImageColor.A);
		Memory::Copy(data, color.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
}

void SpriteRenderer::PrepareSpriteAtlasInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<TransformAtlas2D>& tdata, vec2 subSize)
{
	const int insCnt = (int)tdata.size();
	const uint16 insStride = 64 + sizeof(vec4);
	uint drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	idata.MissedAmount = insCnt - drawnIns;
	idata.pSprite = sprite;

	bgfx::allocInstanceDataBuffer(&idata.Buffer, drawnIns, insStride);

	uint8* data = idata.Buffer.data;

	for (uint i = 0; i < drawnIns; i++)
	{
		auto& transf = tdata[i];
		vec2 rscale = subSize * transf.Scale;

		vec3 rotPiv = vec3(
			sprite->RotationPivot.X * rscale.X,
			sprite->RotationPivot.Y * rscale.Y,
			0.0f
		);

		mat4 mdl = mat4::Identity();
		mdl = Math::Translate(mdl, vec3(transf.Location.X, transf.Location.Y, 0.0f), false);				// move to world pos
		mdl = Math::Translate(mdl, rotPiv, false);															// mov pivot to origin
		mdl = Math::Rotate(mdl, vec3(0.0f, 0.0f, 1.0f), Math::ToRadians(transf.Rotation), false);			// rotate
		mdl = Math::Translate(mdl, -rotPiv, false);															// move pivot back
		mdl = Math::Scale(mdl, vec3(rscale.X, rscale.Y, 1.0f), false);										// scale

		// sneak in blue value in unused z value
		vec4& csp = *((vec4*)(&mdl.Data[0]));
		csp.Z = transf.ImageColor.B;

		// sneak in alpha value in unused z value
		vec4& csp2 = *((vec4*)(&mdl.Data[4]));
		csp2.Z = transf.ImageColor.A;

		const float* _mtx = mdl.Ptr();
		Memory::Copy(data, _mtx, insStride);
		data += 64;

		vec4 color = vec4(transf.Index.X, transf.Index.Y, transf.ImageColor.R, transf.ImageColor.G);
		Memory::Copy(data, color.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
}

void SpriteRenderer::DrawSpriteInstanced(SpriteInstanceData& idata)
{
	bgfx::setState(TUDO_RENDERER_SPRITE_STATE);
	bgfx::setInstanceDataBuffer(&idata.Buffer);

	Texture* usetex = idata.pSprite->GetTexture();
	if (pQuad2DLastTex != usetex)
	{
		pQuad2DLastTex = usetex;
		pActiveShader->SetTexture(0, "s_texColor", *usetex);
	}

	pActiveShader->Submit(pActiveDrawSurface->ViewID(), TUDO_RENDERER_SPRITE_FLAGS, true);
}

void SpriteRenderer::DrawSpriteAtlasInstanced(SpriteInstanceData& idata, Sprite* sprite, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(0.0f, 0.0f, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pActiveShader->SetUniform("atlasInfo", atinf, 2);
	DrawSpriteInstanced(idata);
}

void SpriteRenderer::PrepareSpriteFontText(SpriteFont& font, Transform2D& transformation, SpriteInstanceData& idata, strgv text)
{
	std::vector<TransformAtlas2D> tdata;

	vec2 cursor = transformation.Location;
	for (auto& it : text)
	{
		TransformAtlas2D result;
		result.Location = cursor;
		result.Rotation = transformation.Rotation;
		result.Scale = transformation.Scale;
		result.ImageColor = transformation.ImageColor;

		if (it == ' ')
		{
			cursor.X += font.GlyphSize.X;
			continue;
		}
		else if (it == '\n')
		{
			cursor.X = transformation.Location.X;
			cursor.Y += font.GlyphSize.Y;
			continue;
		}

		auto pos = font.Glyphs.find(it);
		if (pos != strg::npos)
			result.Index.X = static_cast<float>(pos);
		else continue;

		tdata.push_back(result);

		cursor.X += font.GlyphSize.X;
	}

	PrepareSpriteAtlasInstancing(font.pSprite, idata, tdata, font.GlyphSize);
}

void SpriteRenderer::DrawSpriteFontText(SpriteFont& font, SpriteInstanceData& idata)
{
	DrawSpriteAtlasInstanced(idata, font.pSprite, font.GlyphSize);
}

void SpriteRenderer::DrawSpriteFontText(SpriteFont& font, Transform2D& transformation, strgv text)
{
	vec2 cursor = transformation.Location;
	for (auto& it : text)
	{
		TransformAtlas2D result;
		result.Location = cursor;
		result.Rotation = transformation.Rotation;
		result.Scale = transformation.Scale;
		result.ImageColor = transformation.ImageColor;

		if (it == ' ')
		{
			cursor.X += font.GlyphSize.X;
			continue;
		}
		else if (it == '\n')
		{
			cursor.X = transformation.Location.X;
			cursor.Y += font.GlyphSize.Y;
			continue;
		}

		auto pos = font.Glyphs.find(it);
		if (pos != strg::npos)
			result.Index.X = static_cast<float>(pos);
		else continue;

		DrawSpriteAtlas(font.pSprite, result, font.GlyphSize);

		cursor.X += font.GlyphSize.X;
	}
}

void SpriteRenderer::DrawSpriteAnimation(Sprite* sprite, Transform2D& transformation, SpriteAnimator* animator)
{
	TransformAtlas2D transf;
	transf.Location = transformation.Location;
	transf.Rotation = transformation.Rotation;
	transf.Scale = transformation.Scale;
	transf.ImageColor = transformation.ImageColor;
	transf.Index = animator->GetCurrentIndex();
	DrawSpriteAtlas(sprite, transf, animator->GetAnimation()->FrameSize);
}

void SpriteRenderer::DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation)
{
	if (pQuad2DLastTex != texture)
	{
		pQuad2DLastTex = texture;
		pActiveShader->SetTexture(0, "s_texColor", *texture);
	}
	pGDevice->DrawTexture(pActiveShader, pActiveDrawSurface, texture, rotpiv, size, transformation);
}
