/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWOBJECT_HPP_
#define DRAWOBJECT_HPP_

namespace Tudo
{
	class GraphicsDevice;

	class DrawObject
	{
	public:
		DrawObject(GraphicsDevice* gdevice);
		GraphicsDevice* GetRenderer();

	protected:
		GraphicsDevice* pGDevice;
	};
}
#endif