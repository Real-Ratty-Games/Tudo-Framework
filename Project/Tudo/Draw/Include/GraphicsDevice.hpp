/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GRAPHICSDEVICE_HPP_
#define GRAPHICSDEVICE_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include <vector>
#include <unordered_map>

#define TUDO_RENDERER_MESH_DEFAULT_DISCARD		(BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS | BGFX_DISCARD_BINDINGS | BGFX_DISCARD_STATE | BGFX_DISCARD_TRANSFORM)
#define TUDO_RENDERER_MESH_TRANSPARENT_STATE	(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_CULL_CCW)
#define TUDO_RENDERER_MESH_DEFAULT_STATE		(BGFX_STATE_WRITE_Z | TUDO_RENDERER_MESH_TRANSPARENT_STATE)
#define TUDO_RENDERER_SPRITE_FLAGS				(BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_STATE | BGFX_DISCARD_TRANSFORM | BGFX_DISCARD_BINDINGS)
#define TUDO_RENDERER_SPRITE_STATE				(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_DEPTH_TEST_LESS)

namespace Tudo
{
	class Window;
	class Shader;
	class Texture;
	class DrawSurface;

	class GraphicsDevice
	{
	public:
		GraphicsDevice(Window& window, DrawAPI api, bool vsync);
		~GraphicsDevice();

		void BeginDraw();
		void EndDraw();

		void OnResize(vec2i size, bool vsync);

		const bgfx::Caps* GetGPUInfo();

		void Printf(vec2i location, uint8 attr, strgv text);
		void DrawTexture(Shader& shader, const DrawSurface& surface, vec2 rotpiv, vec2 size, const Transform2D& transformation);
		void SetMesh(uint8 stream, const Mesh3D& mesh);
		void SetModelTransform(const mat4& mat);

		bgfx::VertexBufferHandle	CreateVertexBuffer(const void* data, uint size, const bgfx::VertexLayout& layout);
		bgfx::IndexBufferHandle		CreateIndexBuffer(const void* data, uint size);

		bgfx::VertexBufferHandle&	GetQuadVertexHandle();
		const mat4&					GetQuad2DView();

		bgfx::VertexLayout&			GetMeshVertexLayout();

		bgfx::UniformHandle*		GetShaderUniform(strgv name);
		void						InitShaderUniform(strgv name, bgfx::UniformType::Enum type, uint16 nmb = 1);
		void						SetShaderUniform(strgv name, const void* vl, uint16 nmb = 1);
		void						SetShaderTexture(uint8 stage, strgv name, Texture& texture);

		friend class AssetLoader;

	private:
		void ReleaseShaderUniforms();
		void InitMesh3DVBLayout();
		void Init2DQuad();
		void Release2DQuad();

	private:
		bgfx::VertexBufferHandle						mQuad2DVB;
		mat4											mQuad2DView;

		bgfx::VertexLayout								mMesh3DVBLayout;
		std::unordered_map<strg, bgfx::UniformHandle>	mShaderUniforms;
	};
}
#endif