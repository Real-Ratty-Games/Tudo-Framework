/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Program.hpp"

using namespace Tudo;

void Program::Run()
{
	if (Initialize())
	{
		while (!bQuit)
		{
			Tick();
			Draw();
		}
	}
	Cleanup();
}

void Program::Quit(bool vl)
{
	bQuit = vl;
}
