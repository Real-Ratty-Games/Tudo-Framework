/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WINDOWCURSOR_HPP_
#define WINDOWCURSOR_HPP_
#include "SystemTypes.hpp"
#include "Window.hpp"

namespace Tudo
{
	class WindowCursor
	{
	public:
		~WindowCursor();

		friend class Window;
		friend class AssetLoader;

	private:
		SDL_Cursor* mCursor;
	};
}
#endif