/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Renderer.hpp"
#include "BigError.hpp"
#include "Memory.hpp"
#include "Window.hpp"
#include "FileSystem.hpp"
#include "Transformation.hpp"
#include "Math.hpp"
#include "Texture.hpp"
#include <bx/math.h>

#define TUDO_RENDERER_SPRITE_FLAGS (BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_STATE | BGFX_DISCARD_TRANSFORM)
#define TUDO_RENDERER_SPRITE_STATE (BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_DEPTH_TEST_LESS)

#define GAMEENGINE_RENDERER_M3D_VERTEXLAYOUT \
	.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)	\
	.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)		\
	.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)	\
	.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)		\
	.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)	\

using namespace Tudo;

Renderer::Renderer()
{
	pActiveShader		= nullptr;
	pActiveDrawSurface	= nullptr;
	pQuad2DLastTex		= nullptr;
	mQuad2DVB			= BGFX_INVALID_HANDLE;
}

Renderer::~Renderer()
{
	Release2DQuad();
	bgfx::shutdown();
}

bool Renderer::Initialize(Window* window, DrawAPI api, bool vsync)
{
	bgfx::Init init;
	init.platformData.nwh = window->GetNativePtr();

	const vec2i wndSize = window->GetSize();
	init.resolution.width = wndSize.X;
	init.resolution.height = wndSize.Y;

	init.type = (bgfx::RendererType::Enum)api;

	if (!bgfx::init(init))
	{
		throw BigError("Failed intializing bgfx!");
		return false;
	}

#if _DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
	bgfx::reset(wndSize.X, wndSize.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE));
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

	Init2DQuad();
	Init3DLayout();
	return true;
}

void Renderer::BeginDraw()
{
	bgfx::touch(0);

#if _DEBUG
	bgfx::dbgTextClear();
#endif
}

void Renderer::EndDraw()
{
	bgfx::frame();
}

void Renderer::OnResize(vec2i size, bool vsync)
{
	bgfx::reset(size.X, size.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE));
} // ignoring bgfx's native msaa on purpose

const bgfx::Caps* Renderer::GetGPUInfo()
{
	return bgfx::getCaps();
}

void Renderer::Printf(vec2i location, uint8 attr, strgv text)
{
#if _DEBUG
	bgfx::dbgTextPrintf(location.X, location.Y, attr, text.data());
#endif
}

void Renderer::SetScissor(vec2i location, vec2i size)
{
	bgfx::setScissor(location.X, location.Y, size.X, size.Y);
}

void Renderer::SetViewScissor(uint16 viewid, vec2i location, vec2i size)
{
	bgfx::setViewScissor(viewid, location.X, location.Y, size.X, size.Y);
}

bgfx::VertexBufferHandle Renderer::CreateVertexBuffer(const void* data, uint size, bgfx::VertexLayout& layout)
{
	return bgfx::createVertexBuffer(bgfx::copy(data, size), layout);
}

bgfx::IndexBufferHandle Renderer::CreateIndexBuffer(const void* data, uint size)
{
	return bgfx::createIndexBuffer(bgfx::copy(data, size));
}

void Renderer::SetActiveShader(Shader* shader)
{
	pActiveShader = shader;
}

Shader* Renderer::GetActiveShader()
{
	return pActiveShader;
}

void Renderer::SetTransform(const mat4& mat)
{
	bgfx::setTransform(mat.Ptr());
}

void Renderer::SetState(uint64 state)
{
	bgfx::setState(state);
}

void Renderer::BeginDrawSprite(DrawSurface2D* surface, Viewport2D& viewport)
{
	pActiveDrawSurface = surface;
	pActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxOrtho(proj, viewport.Location.X, viewport.Size.X + viewport.Location.Y,
		viewport.Size.Y + viewport.Location.Y, viewport.Location.Y, 0.1f, 100.0f, 0.0f, false);
	bgfx::setViewTransform(pActiveDrawSurface->ViewID(), mQuad2DView, proj);

	bgfx::setVertexBuffer(0, mQuad2DVB);
}

void Renderer::EndDrawSprite()
{
	pQuad2DLastTex = nullptr;
	ClearActives();
}

void Renderer::DrawSprite(Sprite* sprite, Transform2D& transformation)
{
	DrawTexture(sprite->GetTexture(), sprite->RotationPivot, sprite->Size, transformation);
}

void Renderer::DrawSpriteAtlas(Sprite* sprite, TransformAtlas2D& transformation, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(transformation.Index.X, transformation.Index.Y, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pActiveShader->SetUniform("atlasInfo", atinf, 2);
	DrawTexture(sprite->GetTexture(), sprite->RotationPivot, subSize, transformation);
}

void Renderer::PrepareSpriteInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<Transform2D>& tdata)
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

void Renderer::PrepareSpriteAtlasInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<TransformAtlas2D>& tdata, vec2 subSize)
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

void Renderer::DrawSpriteInstanced(SpriteInstanceData& idata)
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

void Renderer::DrawSpriteAtlasInstanced(SpriteInstanceData& idata, Sprite* sprite, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(0.0f, 0.0f, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pActiveShader->SetUniform("atlasInfo", atinf, 2);
	DrawSpriteInstanced(idata);
}

void Renderer::PrepareSpriteFontText(SpriteFont& font, Transform2D& transformation, SpriteInstanceData& idata, strgv text)
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

void Renderer::DrawSpriteFontText(SpriteFont& font, SpriteInstanceData& idata)
{
	DrawSpriteAtlasInstanced(idata, font.pSprite, font.GlyphSize);
}

void Renderer::DrawSpriteFontText(SpriteFont& font, Transform2D& transformation, strgv text)
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

void Renderer::DrawSpriteAnimation(Sprite* sprite, Transform2D& transformation, SpriteAnimator* animator)
{
	TransformAtlas2D transf;
	transf.Location		= transformation.Location;
	transf.Rotation		= transformation.Rotation;
	transf.Scale		= transformation.Scale;
	transf.ImageColor	= transformation.ImageColor;
	transf.Index		= animator->GetCurrentIndex();
	DrawSpriteAtlas(sprite, transf, animator->GetAnimation()->FrameSize);
}

void Renderer::BeginDrawMesh(DrawSurface3D* surface, Viewport3D& viewport)
{
	pActiveDrawSurface = surface;
	pActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxProj(proj, viewport.Fov, surface->AspectRatio, viewport.Near, viewport.Far,
		bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj);
}

void Renderer::BeginDrawMesh(DrawSurface3D* surface, ViewportOrtho3D& viewport)
{
	pActiveDrawSurface = surface;
	pActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxOrtho(proj, viewport.Left, viewport.Right, viewport.Bottom, viewport.Top, viewport.Near,
		viewport.Far, viewport.Offset, true);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj);
}

void Renderer::EndDrawMesh()
{
	ClearActives();
}

void Renderer::SetMesh(uint8 stream, Mesh3D& mesh)
{
	bgfx::setVertexBuffer(stream, mesh.VBH);
	bgfx::setIndexBuffer(mesh.IBH);
}

void Renderer::DrawMesh(uint8 flags)
{
	pActiveShader->Submit(pActiveDrawSurface->ViewID(), flags, true);
}

inline void Renderer::ClearActives()
{
	pActiveShader = nullptr;
	pActiveDrawSurface = nullptr;
}

void Renderer::Init3DLayout()
{
	mMesh3DVBLayout.begin()
		GAMEENGINE_RENDERER_M3D_VERTEXLAYOUT
		.end();
}

void Renderer::Init2DQuad()
{
	static QuadVertex quadVertices[] = {
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f}
	};

	bgfx::VertexLayout pcvLayout;
	pcvLayout.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	mQuad2DVB = Renderer::CreateVertexBuffer(quadVertices, sizeof(quadVertices), pcvLayout);

	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	bx::mtxLookAt(mQuad2DView, eye, at);
}

void Renderer::Release2DQuad()
{
	if (bgfx::isValid(mQuad2DVB))
	{
		bgfx::destroy(mQuad2DVB);
		mQuad2DVB = BGFX_INVALID_HANDLE;
	}
}

void Renderer::DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation)
{
	vec2 rscale = size * transformation.Scale;

	vec3 rotPiv = vec3(
		rotpiv.X * rscale.X,
		rotpiv.Y * rscale.Y,
		0.0f
	);

	mat4 mdl = mat4::Identity();
	mdl = Math::Translate(mdl, vec3(transformation.Location.X, transformation.Location.Y, 0.0f), false);		// move to world pos
	mdl = Math::Translate(mdl, rotPiv, false);																	// mov pivot to origin
	mdl = Math::Rotate(mdl, vec3(0.0f, 0.0f, 1.0f), Math::ToRadians(transformation.Rotation), false);			// rotate
	mdl = Math::Translate(mdl, -rotPiv, false);																	// move pivot back
	mdl = Math::Scale(mdl, vec3(rscale.X, rscale.Y, 1.0f), false);												// scale

	bgfx::setTransform(mdl.Ptr());
	bgfx::setState(TUDO_RENDERER_SPRITE_STATE);

	vec4 ucolor = vec4(transformation.ImageColor.R, transformation.ImageColor.G, transformation.ImageColor.B, transformation.ImageColor.A);
	pActiveShader->SetUniform("color", ucolor.Ptr());

	if (pQuad2DLastTex != texture)
	{
		pQuad2DLastTex = texture;
		pActiveShader->SetTexture(0, "s_texColor", *texture);
	}

	pActiveShader->Submit(pActiveDrawSurface->ViewID(), TUDO_RENDERER_SPRITE_FLAGS, true);
}
