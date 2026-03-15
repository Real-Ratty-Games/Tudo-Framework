/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWOBJECT_HPP_
#define DRAWOBJECT_HPP_

namespace Tudo
{
	class Renderer;

	class DrawObject
	{
	public:
		DrawObject(Renderer* renderer);
		Renderer* GetRenderer();

	protected:
		Renderer* pRenderer;
	};
}
#endif