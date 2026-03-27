/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GAMEPAD_HPP_
#define GAMEPAD_HPP_
#include "SystemTypes.hpp"
#include "InputTypes.hpp"
#include <vector>

class SDL_Gamepad;

namespace Tudo
{
	/// Manages gamepad functionalities
	class Gamepad
	{
    public:
        /// Load gamepad mapping for a bunch of different controller types
        static void LoadConfig();

        ~Gamepad();

        bool        Connect(uint8 port);
        void        Disconnect();
        bool        IsConnected();
        bool        ButtonDown(GamepadButton button);
        bool        ButtonUp(GamepadButton button);
        bool        ButtonPressed(GamepadButton button);

        /// Gamepad motor rumble
        /// <param name="left">from 0 to 0xFFFF (65535)</param>
        /// <param name="right">from 0 to 0xFFFF (65535)</param>
        /// <param name="time">in ms</param>
        void        Rumble(uint16 left, uint16 right, uint time = 0);

        /// Gamepad motor rumble for triggers
        /// <param name="left">from 0 to 0xFFFF (65535)</param>
        /// <param name="right">from 0 to 0xFFFF (65535)</param>
        /// <param name="time">in ms</param>
        void        RumbleTriggers(uint16 left, uint16 right, uint time = 0);

        /// Get axis button state
        /// <param name="axis">thumbsticks: -32768 to 32767; triggers: 0 to 32767</param>
        /// <returns>Axis state</returns>
        int16       Axis(GamepadAxis axis);

    private:
        bool CheckButton(SDL_Gamepad* handle, GamepadButton button);

    private:
        SDL_Gamepad* mGamepadHandle = nullptr;
        std::vector<GamepadButton> mPressed;
	};
}
#endif