/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SPRITE_HPP_
#define SPRITE_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"

namespace GameEngine
{
	class Texture;

	/// <summary>
	/// 2D Quad Sprite
	/// </summary>
	class Sprite
	{
	public:
		Sprite(Texture* texture);
		void SetTexture(Texture* texture);
		Texture* GetTexture();

	public:
		vec2		RotationPivot;
		vec2		Size;

	private:
		Texture*	pTexture;
	};
}
#endif