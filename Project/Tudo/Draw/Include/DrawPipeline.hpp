/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWPIPELINE_HPP_
#define DRAWPIPELINE_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "DrawObject.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawSurface3D;
	class Viewport3D;
	class ViewportOrtho3D;

	/// Abstract base class for render pipelines.
	class DrawPipeline : public DrawObject
	{
	public:
		DrawPipeline(GraphicsDevice* gdevice);

		virtual void Draw() = 0;
		virtual void OnResize(const vec2& size) = 0;

	protected:
		void PrepareDrawModel(DrawSurface3D* surface, Viewport3D& viewport);
		void PrepareDrawModel(DrawSurface3D* surface, ViewportOrtho3D& viewport);
	};
}
#endif