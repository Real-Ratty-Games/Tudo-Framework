/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef RENDERER_HPP_
#define RENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawObject.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawSurface;
	class Shader;

	class Renderer : public DrawObject
	{
	public:
		Renderer(GraphicsDevice* gdevice);

		void SetActiveShader(Shader* shader);
		void SetActiveSurface(DrawSurface* surface);
		void ClearActives();

	protected:
		Shader*			pActiveShader;
		DrawSurface*	pActiveDrawSurface;
	};
}
#endif