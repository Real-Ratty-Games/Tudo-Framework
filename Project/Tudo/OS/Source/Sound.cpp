/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Sound.hpp"
#include "BigError.hpp"

using namespace Tudo;

void SoundManager::Initialize()
{
	mCore = new SoundCore();
	SoundResult err = mCore->init();
	if (err != SoLoud::SO_NO_ERROR)
		throw BigError("Failed initializing sound engine!");
	mCore->set3dListenerUp(0.0f, 1.0f, 0.0f);
}

SoundManager::~SoundManager()
{
	mCore->deinit();
	delete mCore;
}

SoundCore* SoundManager::Core()
{
	return mCore;
}
