/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Sprite.hpp"
#include "BigError.hpp"
#include "Texture.hpp"

using namespace GameEngine;

Sprite::Sprite(Texture* texture)
{
	if (texture == nullptr)
		throw BigError("Sprite creation failed: Texture cannot be nullptr!");
	pTexture	= texture;
	Size		= pTexture->Size();
}

void Sprite::SetTexture(Texture* texture)
{
	if (texture == nullptr)
		throw BigError("Texture cannot be nullptr!");
	pTexture = texture;
}

Texture* Sprite::GetTexture()
{
	return pTexture;
}
