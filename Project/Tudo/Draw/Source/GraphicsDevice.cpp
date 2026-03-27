/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "GraphicsDevice.hpp"
#include "BigError.hpp"
#include "Memory.hpp"
#include "FileSystem.hpp"
#include "Math.hpp"
#include "Transformation.hpp"
#include "Projection.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "DrawSurface.hpp"

#define TUDO_RENDERER_M3D_VERTEXLAYOUT \
	.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)	\
	.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)		\
	.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)	\
	.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)		\
	.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)	\

using namespace Tudo;

GraphicsDevice::GraphicsDevice(Window& window, DrawAPI api, bool vsync)
{
	mQuad2DVB = BGFX_INVALID_HANDLE;

	bgfx::Init init;
	init.platformData.nwh = window.GetNativePtr();

	const vec2i wndSize = window.GetSize();
	init.resolution.width = wndSize.X;
	init.resolution.height = wndSize.Y;

	init.type = (bgfx::RendererType::Enum)api;

	if (!bgfx::init(init))
		throw BigError("Failed intializing bgfx!");

#if _DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
	bgfx::reset(wndSize.X, wndSize.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE));
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

	Init2DQuad();
	InitMesh3DVBLayout();
}

GraphicsDevice::~GraphicsDevice()
{
	Release2DQuad();
	bgfx::shutdown();
}

void GraphicsDevice::BeginDraw()
{
	bgfx::touch(0);

#if _DEBUG
	bgfx::dbgTextClear();
#endif
}

void GraphicsDevice::EndDraw()
{
	bgfx::frame();
}

void GraphicsDevice::OnResize(vec2i size, bool vsync)
{
	bgfx::reset(size.X, size.Y, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE));
} // ignoring bgfx's native msaa on purpose

const bgfx::Caps* GraphicsDevice::GetGPUInfo()
{
	return bgfx::getCaps();
}

void GraphicsDevice::Printf(vec2i location, uint8 attr, strgv text)
{
#if _DEBUG
	bgfx::dbgTextPrintf(location.X, location.Y, attr, text.data());
#endif
}

// bgfx::setViewClear(viewid, flags, color.ToInt(), 1.0f, 0);

void GraphicsDevice::DrawTexture(Shader& shader, const DrawSurface& surface, vec2 rotpiv, vec2 size, const Transform2D& transformation)
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
	shader.SetUniform("color", ucolor.Ptr());

	shader.Submit(surface.ViewID(), TUDO_RENDERER_SPRITE_FLAGS, true);
}

void GraphicsDevice::SetMesh(uint8 stream, const Mesh3D& mesh)
{
	bgfx::setVertexBuffer(stream, mesh.VBH);
	bgfx::setIndexBuffer(mesh.IBH);
}

void GraphicsDevice::SetModelTransform(const mat4& mat)
{
	bgfx::setTransform(mat.Ptr());
}

bgfx::VertexBufferHandle GraphicsDevice::CreateVertexBuffer(const void* data, uint size, const bgfx::VertexLayout& layout)
{
	return bgfx::createVertexBuffer(bgfx::copy(data, size), layout);
}

bgfx::IndexBufferHandle GraphicsDevice::CreateIndexBuffer(const void* data, uint size)
{
	return bgfx::createIndexBuffer(bgfx::copy(data, size));
}

bgfx::VertexBufferHandle& GraphicsDevice::GetQuadVertexHandle()
{
	return mQuad2DVB;
}

const mat4& GraphicsDevice::GetQuad2DView()
{
	return mQuad2DView;
}

bgfx::VertexLayout& GraphicsDevice::GetMeshVertexLayout()
{
	return mMesh3DVBLayout;
}

void GraphicsDevice::Init2DQuad()
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

	mQuad2DVB = CreateVertexBuffer(quadVertices, sizeof(quadVertices), pcvLayout);

	const vec3 eye(0.0f, 0.0f, -5.0f);
	const vec3 target(0.0f, 0.0f, 0.0f);
	mQuad2DView = Math::LookAtLH(eye, target, vec3(.0f, .1f, .0f));
}

void GraphicsDevice::Release2DQuad()
{
	if (bgfx::isValid(mQuad2DVB))
	{
		bgfx::destroy(mQuad2DVB);
		mQuad2DVB = BGFX_INVALID_HANDLE;
	}
}

void GraphicsDevice::InitMesh3DVBLayout()
{
	mMesh3DVBLayout.begin()
		TUDO_RENDERER_M3D_VERTEXLAYOUT
		.end();
}
