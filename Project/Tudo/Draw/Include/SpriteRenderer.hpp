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
	class Texture;
	class Sprite;

	class SpriteRenderer : public Renderer
	{
	public:
		SpriteRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);

		void DrawSprite(Sprite& sprite, const Transform2D& transformation);
		void DrawSpriteAtlas(Sprite& sprite, const TransformAtlas2D& transformation, vec2 subSize);

		void PrepareSpriteInstancing(Sprite& sprite, SpriteInstanceData& idata, const std::vector<Transform2D>& tdata);
		void PrepareSpriteAtlasInstancing(Sprite& sprite, SpriteInstanceData& idata, const std::vector<TransformAtlas2D>& tdata, vec2 subSize);

		void DrawSpriteInstanced(const SpriteInstanceData& idata);
		void DrawSpriteAtlasInstanced(const SpriteInstanceData& idata, Sprite& sprite, vec2 subSize);

		void PrepareSpriteFontText(const SpriteFont& font, const Transform2D& transformation, SpriteInstanceData& idata, strgv text);
		void DrawSpriteFontText(const SpriteFont& font, const SpriteInstanceData& idata);
		void DrawSpriteFontText(const SpriteFont& font, const Transform2D& transformation, strgv text);

		void DrawSpriteAnimation(Sprite& sprite, const Transform2D& transformation, const SpriteAnimator& animator);

		void DrawColorQuad(const Transform2D& transformation, vec2 rotpiv, vec2 size);

	private:
		void DrawTexture(Texture* texture, vec2 rotpiv, vec2 size, const Transform2D& transformation);

	private:
		Texture* pQuad2DLastTex; // don't update sample if it's the same!
	};
}
#endif