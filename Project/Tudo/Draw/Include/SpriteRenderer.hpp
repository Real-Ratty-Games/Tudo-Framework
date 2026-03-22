/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SPRITERENDERER_HPP_
#define SPRITERENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Renderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class SpriteAnimator;
	class DrawPipeline;
	class DrawSurface2D;
	class Texture;
	class Sprite;

	class SpriteRenderer : public Renderer
	{
	public:
		SpriteRenderer(GraphicsDevice* gdevice, DrawPipeline* pipeline);

		void BeginDrawSprite(Viewport2D& viewport);
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

	private:
		void DrawTexture(Texture* texture, vec2& rotpiv, vec2& size, Transform2D& transformation);

	private:
		Texture*		pQuad2DLastTex;		// don't update sample if it's the same!
		mat4			mQuad2DView;
	};
}
#endif