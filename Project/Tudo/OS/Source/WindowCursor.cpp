/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "WindowCursor.hpp"
#include "BigError.hpp"

using namespace Tudo;

WindowCursor::~WindowCursor()
{
	SDL_DestroyCursor(mCursor);
}
