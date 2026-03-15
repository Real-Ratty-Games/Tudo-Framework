/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GAMEWINDOW_HPP_
#define GAMEWINDOW_HPP_
#include <SystemTypes.hpp>
#include <Window.hpp>
#include <Program.hpp>

using namespace Tudo;

namespace MyGame
{
	class GameWindow : public Window
	{
	public:
		GameWindow(Program* program);

	protected:
		void EventCallback();

	private:
		Program*		pProgram;
	};
}
#endif