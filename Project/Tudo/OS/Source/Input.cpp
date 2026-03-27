/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Input.hpp"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

using namespace Tudo;

bool Input::KeyboardKeyDown(KeyboardKey key)
{
	return CheckKeyboardKey((int)key);
}

bool Input::KeyboardKeyUp(KeyboardKey key)
{
	return !KeyboardKeyDown(key);
}

bool Input::KeyboardKeyPressed(KeyboardKey key)
{
	const bool down = CheckKeyboardKey((int)key);
	auto it = std::find(mPressedKeyboardKeys.begin(), mPressedKeyboardKeys.end(), key);
	const bool pressed = (it != mPressedKeyboardKeys.end());

	if (down && !pressed)
	{
		mPressedKeyboardKeys.push_back(key);
		return true;
	}
	else if (!down && pressed) mPressedKeyboardKeys.erase(it);
	return false;
}

bool Input::MouseButtonDown(MouseButton button)
{
	return CheckMouseButton((int)button);
}

bool Input::MouseButtonUp(MouseButton button)
{
	return !MouseButtonDown(button);
}

bool Input::MouseButtonPressed(MouseButton button)
{
	const bool down = CheckMouseButton((int)button);
	auto it = std::find(mPressedMouseButtons.begin(), mPressedMouseButtons.end(), button);
	const bool pressed = (it != mPressedMouseButtons.end());

	if (down && !pressed)
	{
		mPressedMouseButtons.push_back(button);
		return true;
	}
	else if (!down && pressed) mPressedMouseButtons.erase(it);
	return false;
}

vec2 Input::MouseCursorLocation()
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	return vec2(x, y);
}

/// Returns true if keyboard key is held down
bool Input::CheckKeyboardKey(int key)
{
	const bool* scode = SDL_GetKeyboardState(nullptr);
	return scode[key];
}

/// Returns true if mouse button is held down
bool Input::CheckMouseButton(int button)
{
	SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
	return (flags == button);
}
