/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Renderer.hpp"
#include "../Include/Globals.hpp"
#include "../Include/BigError.hpp"
#include "../Include/Window.hpp"
#include "../Include/FileSystem.hpp"
#include "../Include/Shader.hpp"
#include "../Include/Sprite.hpp"
#include "../Include/Viewport3D.hpp"
#include "../Include/DrawSurface.hpp"
#include "../Include/DrawSurface2D.hpp"
#include "../Include/DrawSurface3D.hpp"
#include "../Include/Transformation.hpp"
#include "../Include/SpriteAnimation.hpp"
#include "../Include/Math.hpp"
#include <bx/math.h>
#include <unordered_set>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#define GAMEENGINE_RENDERER_SPRITE_FLAGS (BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_STATE | BGFX_DISCARD_TRANSFORM)
#define GAMEENGINE_RENDERER_SPRITE_STATE (BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_DEPTH_TEST_LESS)

using namespace GameEngine;

/// Active instances for rendering
static Shader*		_ActiveShader		= nullptr;
static DrawSurface*	_ActiveDrawSurface	= nullptr;

static bgfx::VertexLayout		_Mesh3DVBLayout;

/// For primitive 2D quad rendering
static bgfx::VertexBufferHandle	_Quad2DVB;
static float					_Quad2DView[16];
static Texture*					_Quad2DLastTex = nullptr; // don't update sample if it's the same!

static inline void Renderer_LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents,
	strgv texturename, int width, int height, bool mipgen);
static inline void Renderer_ClearActives();
static void	Renderer_Init3DLayout();
static void	Renderer_Init2DQuad();
static void	Renderer_Release2DQuad();
static void	Renderer_DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation);
static void	Renderer_CreateMesh(Mesh3D& modelMesh, MeshData& mdata);

bool Renderer::Initialize(Window* window, DrawAPI api, bool vsync, MSAA msaa)
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
	bgfx::reset(wndSize.X, wndSize.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE) | (uint)msaa);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

	Renderer_Init2DQuad();
	Renderer_Init3DLayout();
	return true;
}

void Renderer::Release()
{
	Renderer_Release2DQuad();
	bgfx::shutdown();
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

void Renderer::OnResize(vec2i size, bool vsync, MSAA msaa)
{
	bgfx::reset(size.X, size.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE) | (uint)msaa);
}

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

void Renderer::LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load(filename.data(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from file: " + strg(filename);
		throw BigError(errmsg);
	}

	Renderer_LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

void Renderer::LoadTextureFromMemory(Texture& texture, std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load_from_memory(memData.data(), memData.size(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		const strg errmsg = "Failed loading texture from memory: " + strg(texturename);
		throw BigError(errmsg);
	}

	Renderer_LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

void Renderer::FreeTexture(Texture& tex)
{
	if (bgfx::isValid(tex.Handle))
	{
		bgfx::destroy(tex.Handle);
		tex.Handle = BGFX_INVALID_HANDLE;
	}
}

void Renderer::SetActiveShader(Shader* shader)
{
	_ActiveShader = shader;
}

Shader* Renderer::GetActiveShader()
{
	return _ActiveShader;
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
	_ActiveDrawSurface = surface;
	_ActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxOrtho(proj, viewport.Location.X, viewport.Size.X + viewport.Location.Y,
		viewport.Size.Y + viewport.Location.Y, viewport.Location.Y, 0.1f, 100.0f, 0.0f, false);
	bgfx::setViewTransform(_ActiveDrawSurface->ViewID(), _Quad2DView, proj);

	bgfx::setVertexBuffer(0, _Quad2DVB);
}

void Renderer::EndDrawSprite()
{
	_Quad2DLastTex = nullptr;
	Renderer_ClearActives();
}

void Renderer::DrawSprite(Sprite* sprite, Transform2D& transformation)
{
	Renderer_DrawTexture(sprite->GetTexture(), sprite->RotationPivot, sprite->Size, transformation);
}

void Renderer::DrawSpriteAtlas(Sprite* sprite, TransformAtlas2D& transformation, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(transformation.Index.X, transformation.Index.Y, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	_ActiveShader->SetUniform("atlasInfo", atinf, 2);
	Renderer_DrawTexture(sprite->GetTexture(), sprite->RotationPivot, subSize, transformation);
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
		memcpy(data, _mtx, insStride);
		data += 64;

		vec4 color = vec4(transf.ImageColor.R, transf.ImageColor.G, transf.ImageColor.B, transf.ImageColor.A);
		memcpy(data, color.Ptr(), sizeof(vec4));
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
		memcpy(data, _mtx, insStride);
		data += 64;

		vec4 color = vec4(transf.Index.X, transf.Index.Y, transf.ImageColor.R, transf.ImageColor.G);
		memcpy(data, color.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
}

void Renderer::DrawSpriteInstanced(SpriteInstanceData& idata)
{
	bgfx::setState(GAMEENGINE_RENDERER_SPRITE_STATE);
	bgfx::setInstanceDataBuffer(&idata.Buffer);

	Texture* usetex = idata.pSprite->GetTexture();
	if (_Quad2DLastTex != usetex)
	{
		_Quad2DLastTex = usetex;
		_ActiveShader->SetTexture(0, "s_texColor", *usetex);
	}

	_ActiveShader->Submit(_ActiveDrawSurface->ViewID(), GAMEENGINE_RENDERER_SPRITE_FLAGS, true);
}

void Renderer::DrawSpriteAtlasInstanced(SpriteInstanceData& idata, Sprite* sprite, vec2 subSize)
{
	vec4 atinf[2];
	atinf[0] = vec4(0.0f, 0.0f, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	_ActiveShader->SetUniform("atlasInfo", atinf, 2);
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

void Renderer::LoadModelFromFile(Model3D& model, strgv filename)
{
	if (!FileSystem::Exists(filename))
	{
		const strg errmsg = "Model file could not be found: " + strg(filename);
		throw BigError(errmsg);
	}

	std::ifstream file(filename.data(), std::ios::binary);
	file.seekg(0, std::ios::beg);

	M3DCHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(M3DCHeader));

	if(header.Magic != M3DC_MAGIC)
	{
		const strg errmsg = "Model file format invalid: " + strg(filename);
		throw BigError(errmsg);
	}

	if (header.Version != M3DC_VERSION)
	{
		const strg errmsg = "Model file format uses older version: " + strg(filename);
		throw BigError(errmsg);
	}

	model.Meshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;
		file.read(reinterpret_cast<char*>(&vsize), sizeof(uint64));
		file.read(reinterpret_cast<char*>(&isize), sizeof(uint64));

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		file.read(reinterpret_cast<char*>(mdata.Vertices.data()), vsize * sizeof(MeshVertex));
		file.read(reinterpret_cast<char*>(mdata.Indices.data()), isize * sizeof(uint16));

		Renderer_CreateMesh(model.Meshes[i], mdata);
	}
}

void Renderer::LoadModelFromMemory(Model3D& model, std::vector<uint8>& data)
{
	if (data.empty())
		throw BigError("Data is empty!");

	const uint8* rdat = data.data();
	uint64 offset = 0;

	M3DCHeader header;
	std::memcpy(&header, rdat + offset, sizeof(M3DCHeader));
	offset += sizeof(M3DCHeader);

	if (header.Magic != M3DC_MAGIC)
		throw BigError("Model file format invalid");

	if (header.Version != M3DC_VERSION)
		throw BigError("Model file format uses older version");

	model.Meshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;

		std::memcpy(&vsize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		std::memcpy(&isize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		const uint64 vtsize = vsize * sizeof(MeshVertex);
		std::memcpy(mdata.Vertices.data(), rdat + offset, vtsize);
		offset += vtsize;

		const uint64 itsize = isize * sizeof(uint16);
		std::memcpy(mdata.Indices.data(), rdat + offset, itsize);
		offset += itsize;

		Renderer_CreateMesh(model.Meshes[i], mdata);
	}
}

void Renderer::FreeModel(Model3D& model)
{
	for (auto& it : model.Meshes)
	{
		if (bgfx::isValid(it.VBH))
			bgfx::destroy(it.VBH);
		if (bgfx::isValid(it.IBH))
			bgfx::destroy(it.IBH);
	}
}

void Renderer::BeginDrawMesh(DrawSurface3D* surface, Viewport3D& viewport)
{
	_ActiveDrawSurface = surface;
	_ActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxProj(proj, viewport.Fov, surface->AspectRatio, viewport.Near, viewport.Far,
		bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj);
}

void Renderer::BeginDrawMesh(DrawSurface3D* surface, ViewportOrtho3D& viewport)
{
	_ActiveDrawSurface = surface;
	_ActiveDrawSurface->Clear();

	float proj[16];
	bx::mtxOrtho(proj, viewport.Left, viewport.Right, viewport.Bottom, viewport.Top, viewport.Near,
		viewport.Far, viewport.Offset, true);
	bgfx::setViewTransform(surface->ViewID(), viewport.View().Ptr(), proj);
}

void Renderer::EndDrawMesh()
{
	Renderer_ClearActives();
}

void Renderer::SetMesh(uint8 stream, Mesh3D& mesh)
{
	bgfx::setVertexBuffer(stream, mesh.VBH);
	bgfx::setIndexBuffer(mesh.IBH);
}

void Renderer::DrawMesh(uint8 flags)
{
	_ActiveShader->Submit(_ActiveDrawSurface->ViewID(), flags, true);
}

/*======================================================
======================================================*/

inline void Renderer_LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents, strgv texturename,
	int width, int height, bool mipgen)
{
	bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
	if (nrComponents == 3)
		format = bgfx::TextureFormat::RGB8;
	else if (nrComponents == 4)
		format = bgfx::TextureFormat::RGBA8;

	if (mipgen)
	{
		int nmips = std::max(width, height);
		nmips = 1 + static_cast<int>(std::floor(std::log2(nmips)));

		texture.Handle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			true,
			1,
			format,
			flags,
			nullptr);

		int mipWidth = width;
		int mipHeight = height;
		stbi_uc* mipData = data;

		for (int i = 0; i < nmips; i++)
		{
			bgfx::updateTexture2D(
				texture.Handle,
				0,
				i,
				0, 0,
				static_cast<uint16>(mipWidth),
				static_cast<uint16>(mipHeight),
				bgfx::copy(mipData, mipWidth * mipHeight * nrComponents));

			mipWidth = std::max(1, mipWidth / 2);
			mipHeight = std::max(1, mipHeight / 2);
			stbi_uc* nextMip = (stbi_uc*)malloc(mipWidth * mipHeight * nrComponents);

			stbir_resize_uint8_linear(mipData, mipWidth * 2, mipHeight * 2, 0, nextMip, mipWidth, mipHeight, 0,
				(stbir_pixel_layout)nrComponents);

			if (mipData != data) free(mipData);
			mipData = nextMip;
		}

		if (mipData != data) free(mipData);
	}
	else
	{
		texture.Handle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			false,
			0,
			format,
			flags,
			bgfx::copy(data, width * height * nrComponents));
	}

	if (bgfx::isValid(texture.Handle))
		bgfx::setName(texture.Handle, texturename.data());
	else
	{
		const strg errmsg = "Failed loading texture: " + strg(texturename);
		throw BigError(errmsg);
	}

	texture.Size = vec2i(width, height);

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
}

inline void Renderer_ClearActives()
{
	_ActiveShader = nullptr;
	_ActiveDrawSurface = nullptr;
}

void Renderer_Init3DLayout()
{
	_Mesh3DVBLayout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
		.end();
}

void Renderer_Init2DQuad()
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

	_Quad2DVB = Renderer::CreateVertexBuffer(quadVertices, sizeof(quadVertices), pcvLayout);

	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	bx::mtxLookAt(_Quad2DView, eye, at);
}

void Renderer_Release2DQuad()
{
	if (bgfx::isValid(_Quad2DVB))
	{
		bgfx::destroy(_Quad2DVB);
		_Quad2DVB = BGFX_INVALID_HANDLE;
	}
}

void Renderer_DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation)
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
	bgfx::setState(GAMEENGINE_RENDERER_SPRITE_STATE);

	vec4 ucolor = vec4(transformation.ImageColor.R, transformation.ImageColor.G, transformation.ImageColor.B, transformation.ImageColor.A);
	_ActiveShader->SetUniform("color", ucolor.Ptr());


	if (_Quad2DLastTex != texture)
	{
		_Quad2DLastTex = texture;
		_ActiveShader->SetTexture(0, "s_texColor", *texture);
	}

	_ActiveShader->Submit(_ActiveDrawSurface->ViewID(), GAMEENGINE_RENDERER_SPRITE_FLAGS, true);
}

void Renderer_CreateMesh(Mesh3D& modelMesh, MeshData& mdata)
{
	modelMesh.VBH = Renderer::CreateVertexBuffer(mdata.Vertices.data(), mdata.VSize * sizeof(MeshVertex), _Mesh3DVBLayout);
	if (!bgfx::isValid(modelMesh.VBH))
		throw BigError("Vertex Buffer is invalid!");

	modelMesh.IBH = Renderer::CreateIndexBuffer(mdata.Indices.data(), mdata.ISize * sizeof(uint16));
	if (!bgfx::isValid(modelMesh.IBH))
		throw BigError("Index Buffer is invalid!");
}
