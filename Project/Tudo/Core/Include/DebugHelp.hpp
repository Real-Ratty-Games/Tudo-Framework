/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DEBUGHELP_HPP_
#define DEBUGHELP_HPP_

#if _MSC_VER
	#include <Windows.h>
	#define DEBUGHELP_LOOK4MEMLEAKS()	{_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);}
	#define DEBUGHELP_SHOWCMD(vl)		{::ShowWindow(::GetConsoleWindow(), vl);}
#else
	#define DEBUGHELP_LOOK4MEMLEAKS()	{}
	#define DEBUGHELP_SHOWCMD			{}
#endif

#endif