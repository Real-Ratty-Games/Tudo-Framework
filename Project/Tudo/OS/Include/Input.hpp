/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef INPUT_HPP_
#define INPUT_HPP_
#include "SystemTypes.hpp"
#include "InputTypes.hpp"
#include <vector>

namespace Tudo
{
	class Input
	{
	public:
		bool KeyboardKeyDown(KeyboardKey key);
		bool KeyboardKeyUp(KeyboardKey key);
		bool KeyboardKeyPressed(KeyboardKey key);
		bool MouseButtonDown(MouseButton button);
		bool MouseButtonUp(MouseButton button);
		bool MouseButtonPressed(MouseButton button);
		vec2 MouseCursorLocation();

	private:
		bool CheckKeyboardKey(int key);
		bool CheckMouseButton(int button);

	private:
		std::vector<KeyboardKey> mPressedKeyboardKeys;
		std::vector<MouseButton> mPressedMouseButtons;
	};
}
#endif