/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWDATA_HPP_
#define DRAWDATA_HPP_
#include "SystemTypes.hpp"
#include "Transformation.hpp"
#if _DEBUG
#define BX_CONFIG_DEBUG 1
#else
#define BX_CONFIG_DEBUG 0
#endif
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <vector>

namespace Tudo
{
	class Sprite;

	enum class DrawAPI
	{
		DIRECT3D11	= bgfx::RendererType::Direct3D11,
		DIRECT3D12	= bgfx::RendererType::Direct3D12,
		VULKAN		= bgfx::RendererType::Vulkan,
        METAL       = bgfx::RendererType::Metal
	};

	/// range: [0, 1]
	struct Color
	{
		float R;
		float G;
		float B;
		float A;

		Color() : R(0), G(0), B(0), A(0) {}

		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

		Color(const Color& other)
		{
			R = other.R;
			G = other.G;
			B = other.B;
			A = other.A;
		}

		Color(uint rgba)
		{
			R = ((rgba >> 16) & 0xff)	/ 255.0f;
			G = ((rgba >> 8) & 0xff)	/ 255.0f;
			B = (rgba & 0xff)			/ 255.0f;
			A = ((rgba >> 24) & 0xff)	/ 255.0f;
		}

		uint ToInt() const
		{
			uint r = (uint)(R * 255);
			uint g = (uint)(G * 255);
			uint b = (uint)(B * 255);
			uint a = (uint)(A * 255);
			return (r << 28) | (g << 16) | (b << 8) | a;
		}

		Color operator=(const uint& rgba)
		{
			return Color(rgba);
		}
	};

	struct QuadVertex
	{
		float X;
		float Y;
		float U;
		float V;
	};

	struct SpriteInstanceData
	{
		Sprite*						pSprite;
		bgfx::InstanceDataBuffer	Buffer;
		int							MissedAmount; // Amount of instances not rendered due to hardware limitations
	};

	struct Viewport2D
	{
		vec2 Location;
		vec2 Size;
	};

	struct Transform2D
	{
		vec2	Location;
		vec2	Scale;
		float	Rotation;
		Color	ImageColor;

		Transform2D()
		{
			Location	= vec2(0.0f);
			Scale		= vec2(1.0f);
			Rotation	= 0.0f;
		}
	};

	struct TransformAtlas2D : public Transform2D
	{
		vec2	Index;

		TransformAtlas2D() : Transform2D()
		{
			Index		= vec2(0.0f);
		}
	};

	struct SpriteFont
	{
		Sprite* pSprite;
		strg	Glyphs;
		vec2	GlyphSize;
	};

	struct Mesh3D
	{
		bgfx::VertexBufferHandle	VBH;
		bgfx::IndexBufferHandle		IBH;
	};
}
#endif
