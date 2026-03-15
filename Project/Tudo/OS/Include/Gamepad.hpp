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
	/// <summary>
	/// Manages gamepad functionalities
	/// </summary>
	class Gamepad
	{
    public:
        /// <summary>
        /// Load gamepad mapping for a bunch of different controller types
        /// </summary>
        static void LoadConfig();

        ~Gamepad();

        bool        Connect(uint8 port);
        void        Disconnect();
        bool        IsConnected();
        bool        ButtonDown(GamepadButton button);
        bool        ButtonUp(GamepadButton button);
        bool        ButtonPressed(GamepadButton button);

        /// <summary>
        /// Gamepad motor rumble
        /// </summary>
        /// <param name="left">from 0 to 0xFFFF (65535)</param>
        /// <param name="right">from 0 to 0xFFFF (65535)</param>
        /// <param name="time">in ms</param>
        void        Rumble(uint16 left, uint16 right, uint time = 0);

        /// <summary>
        /// Gamepad motor rumble for triggers
        /// </summary>
        /// <param name="left">from 0 to 0xFFFF (65535)</param>
        /// <param name="right">from 0 to 0xFFFF (65535)</param>
        /// <param name="time">in ms</param>
        void        RumbleTriggers(uint16 left, uint16 right, uint time = 0);

        /// <summary>
        /// Get axis button state
        /// </summary>
        /// <param name="axis">thumbsticks: -32768 to 32767; triggers: 0 to 32767</param>
        /// <returns>Axis state</returns>
        int16       Axis(GamepadAxis axis);

    private:
        SDL_Gamepad* mGamepadHandle = nullptr;
        std::vector<GamepadButton> mPressed;
	};
}
#endif