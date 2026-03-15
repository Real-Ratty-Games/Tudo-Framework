/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "BigError.hpp"
#include "Window.hpp"

using namespace Tudo;

BigError::BigError(const strg& message) : std::runtime_error(message)
{
	if (Window::IsSDLInit())
	{
		if (Window::IsSplashScreenVisible())
			Window::DestroySplashScreen();
#if !_DEBUG
		Window::ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message);
#endif
	}
}
