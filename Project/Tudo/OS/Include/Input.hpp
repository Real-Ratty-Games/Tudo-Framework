/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_
#include "SystemTypes.hpp"
#include "InputTypes.hpp"

namespace Tudo
{
	/// <summary>
	/// Keyboard input functions
	/// </summary>
	namespace Keyboard
	{
		bool	KeyDown(KeyboardKey key);
		bool	KeyUp(KeyboardKey key);
		bool	KeyPressed(KeyboardKey key);
	}

	/// <summary>
	/// Mouse input functions
	/// </summary>
	namespace Mouse
	{
		bool	ButtonDown(MouseButton button);
		bool	ButtonUp(MouseButton button);
		bool	ButtonPressed(MouseButton button);
		vec2	CursorLocation();
	}
}
#endif