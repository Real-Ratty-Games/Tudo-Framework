/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef RENDERER_HPP_
#define RENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include <vector>

#define GAMEENGINE_RENDERER_MESH_STATE (BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS | BGFX_DISCARD_STATE)

namespace GameEngine
{
	class Window;
	class Shader;
	class Sprite;
	class Viewport3D;
	class DrawSurface2D;
	class DrawSurface3D;
	class SpriteAnimator;
	class ViewportOrtho3D;

	namespace Renderer
	{
		bool Initialize(Window* window, DrawAPI api, bool vsync, MSAA msaa);
		void Release();
		void BeginDraw();
		void EndDraw();

		void OnResize(vec2i size, bool vsync, MSAA msaa);

		const bgfx::Caps* GetGPUInfo();

		/// 0x0f
		void Printf(vec2i location, uint8 attr, strgv text);

		void SetScissor(vec2i location, vec2i size);
		void SetViewScissor(uint16 viewid, vec2i location, vec2i size);

		bgfx::VertexBufferHandle CreateVertexBuffer(const void* data, uint size, bgfx::VertexLayout& layout);
		bgfx::IndexBufferHandle CreateIndexBuffer(const void* data, uint size);

		// For uncompressed textures
		void LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen);
		void LoadTextureFromMemory(Texture& texture, std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen);

		// For GPU friendly textures
		// DDS on Windows and KTX on macOS
		void LoadGPUTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename);
		void LoadGPUTextureFromMemory(Texture& texture, std::vector<uint8>& memData, uint64 flags, strgv texturename);

		void FreeTexture(Texture& tex);

		void SetActiveShader(Shader* shader);
		Shader* GetActiveShader();

		void SetTransform(const mat4& mat);
		void SetState(uint64 state);

		// Begin Sprite Rendering

		void BeginDrawSprite(DrawSurface2D* surface, Viewport2D& viewport);
		void EndDrawSprite();

		void DrawSprite(Sprite* sprite, Transform2D& transformation);
		void DrawSpriteAtlas(Sprite* sprite, TransformAtlas2D& transformation, vec2 subSize);

		void PrepareSpriteInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<Transform2D>& tdata);
		void PrepareSpriteAtlasInstancing(Sprite* sprite, SpriteInstanceData& idata, std::vector<TransformAtlas2D>& tdata, vec2 subSize);
			
		void DrawSpriteInstanced(SpriteInstanceData& idata);
		void DrawSpriteAtlasInstanced(SpriteInstanceData& idata, Sprite* sprite, vec2 subSize);

		void PrepareSpriteFontText(SpriteFont& font, Transform2D& transformation, SpriteInstanceData& idata, strgv text);
		void DrawSpriteFontText(SpriteFont& font, SpriteInstanceData& idata);
		void DrawSpriteFontText(SpriteFont& font, Transform2D& transformation, strgv text);

		void DrawSpriteAnimation(Sprite* sprite, Transform2D& transformation, SpriteAnimator* animator);

		// End Sprite Rendering

		void LoadModelFromFile(Model3D& model, strgv filename);
		void LoadModelFromMemory(Model3D& model, std::vector<uint8>& data);
		void FreeModel(Model3D& model);

		// Begin 3D Rendering

		void BeginDrawMesh(DrawSurface3D* surface, Viewport3D& viewport);
		void BeginDrawMesh(DrawSurface3D* surface, ViewportOrtho3D& viewport);
		void EndDrawMesh();

		void SetMesh(uint8 stream, Mesh3D& mesh);
		void DrawMesh(uint8 flags = GAMEENGINE_RENDERER_MESH_STATE);

		// End 3D Rendering
	}
}
#endif