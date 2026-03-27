/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Sprite.hpp"
#include "BigError.hpp"
#include "Texture.hpp"

using namespace Tudo;

Sprite::Sprite(Texture& texture)
{
	pTexture	= &texture;
	Size		= pTexture->Size();
}

void Sprite::SetTexture(Texture& texture)
{
	pTexture = &texture;
}

Texture* Sprite::GetTexture() const
{
	return pTexture;
}
