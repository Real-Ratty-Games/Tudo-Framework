/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GRAPHICSDEVICE_HPP_
#define GRAPHICSDEVICE_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include <vector>

#define TUDO_RENDERER_MESH_DEFAULT_STATE	(BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS | BGFX_DISCARD_STATE)
#define TUDO_RENDERER_SPRITE_FLAGS			(BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_STATE | BGFX_DISCARD_TRANSFORM)
#define TUDO_RENDERER_SPRITE_STATE			(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_DEPTH_TEST_LESS)

namespace Tudo
{
	class Window;
	class Shader;
	class Texture;
	class DrawSurface;

	class GraphicsDevice
	{
	public:
		GraphicsDevice(Window* window, DrawAPI api, bool vsync);
		~GraphicsDevice();

		void BeginDraw();
		void EndDraw();

		void OnResize(vec2i size, bool vsync);

		const bgfx::Caps* GetGPUInfo();

		void Printf(vec2i location, uint8 attr, strgv text);
		void DrawTexture(Shader* shader, DrawSurface* surface, Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation);
		void SetMesh(uint8 stream, const Mesh3D& mesh);

		bgfx::VertexBufferHandle	CreateVertexBuffer(const void* data, uint size, bgfx::VertexLayout& layout);
		bgfx::IndexBufferHandle		CreateIndexBuffer(const void* data, uint size);

		bgfx::VertexBufferHandle&	GetQuadVertexHandle();
		bgfx::VertexLayout&			GetMeshVertexLayout();

		friend class AssetLoader;

	private:
		void InitMesh3DVBLayout();
		void Init2DQuad();
		void Release2DQuad();

	private:
		bgfx::VertexBufferHandle	mQuad2DVB;
		bgfx::VertexLayout			mMesh3DVBLayout;
	};
}
#endif