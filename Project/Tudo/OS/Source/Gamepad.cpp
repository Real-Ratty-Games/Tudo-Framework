/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Gamepad.hpp"
#include "BigError.hpp"
#include <SDL3/SDL_gamepad.h>

using namespace Tudo;

void Gamepad::LoadConfig()
{
	if (SDL_AddGamepadMappingsFromFile("Data/Gamepads.ini") == -1)
	{
		const strg errmsg = "Failed adding gamepad mapping: " + strg(SDL_GetError());
		throw BigError(errmsg);
	}
}

Gamepad::~Gamepad()
{
	if(IsConnected())
		Disconnect();
}

bool Gamepad::Connect(uint8 port)
{
	bool rtvl = true;
	if (!SDL_HasGamepad()) rtvl = false;
	else
	{
		int cnt = 0;
		SDL_JoystickID* ids = SDL_GetGamepads(&cnt);
		if (cnt <= port) rtvl = false;
		else
		{
			mGamepadHandle = SDL_OpenGamepad(ids[port]);
			rtvl = SDL_GamepadConnected(mGamepadHandle);
		}
		SDL_free(ids);
	}
	return rtvl;
}

void Gamepad::Disconnect()
{
	SDL_CloseGamepad(mGamepadHandle);
	mGamepadHandle = nullptr;
}

bool Gamepad::IsConnected()
{
	return SDL_GamepadConnected(mGamepadHandle);
}

bool Gamepad::ButtonDown(GamepadButton button)
{
	return CheckButton(mGamepadHandle, button);
}

bool Gamepad::ButtonUp(GamepadButton button)
{
	return !ButtonDown(button);
}

bool Gamepad::ButtonPressed(GamepadButton button)
{
	const bool down = CheckButton(mGamepadHandle, button);
	auto it = std::find(mPressed.begin(), mPressed.end(), button);
	const bool pressed = (it != mPressed.end());

	if (down && !pressed)
	{
		mPressed.push_back(button);
		return true;
	}
	else if (!down && pressed) mPressed.erase(it);
	return false;
}

void Gamepad::Rumble(uint16 left, uint16 right, uint time)
{
	SDL_RumbleGamepad(mGamepadHandle, left, right, time);
}

void Gamepad::RumbleTriggers(uint16 left, uint16 right, uint time)
{
	SDL_RumbleGamepadTriggers(mGamepadHandle, left, right, time);
}

int16 Gamepad::Axis(GamepadAxis axis)
{
	return SDL_GetGamepadAxis(mGamepadHandle, (SDL_GamepadAxis)axis);
}

/// Returns true if gamepad button is held down
bool Gamepad::CheckButton(SDL_Gamepad* handle, GamepadButton button)
{
	return SDL_GetGamepadButton(handle, (SDL_GamepadButton)button);
}
