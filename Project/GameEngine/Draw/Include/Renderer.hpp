/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef RENDERER_HPP_
#define RENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Globals.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Viewport3D.hpp"
#include "DrawSurface.hpp"
#include "DrawSurface2D.hpp"
#include "DrawSurface3D.hpp"
#include "SpriteAnimation.hpp"
#include <vector>

#define GAMEENGINE_RENDERER_MESH_STATE (BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS | BGFX_DISCARD_STATE)

namespace GameEngine
{
	class Window;
	class Shader;
	class Texture;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		bool Initialize(Window* window, DrawAPI api, bool vsync);
		void BeginDraw();
		void EndDraw();

		void OnResize(vec2i size, bool vsync);

		const bgfx::Caps* GetGPUInfo();

		/// 0x0f
		void Printf(vec2i location, uint8 attr, strgv text);

		void SetScissor(vec2i location, vec2i size);
		void SetViewScissor(uint16 viewid, vec2i location, vec2i size);

		bgfx::VertexBufferHandle CreateVertexBuffer(const void* data, uint size, bgfx::VertexLayout& layout);
		bgfx::IndexBufferHandle CreateIndexBuffer(const void* data, uint size);

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

	private:
		inline void ClearActives();
		void		Init3DLayout();
		void		Init2DQuad();
		void		Release2DQuad();
		void		DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation);
		void		CreateMesh(Mesh3D& modelMesh, MeshData& mdata);

	private:
		/// Active instances for rendering
		Shader*						pActiveShader;
		DrawSurface*				pActiveDrawSurface;

		bgfx::VertexLayout			mMesh3DVBLayout;

		/// For primitive 2D quad rendering
		bgfx::VertexBufferHandle	mQuad2DVB;
		float						mQuad2DView[16];

		// don't update sample if it's the same!
		Texture*					pQuad2DLastTex;
	};
}
#endif