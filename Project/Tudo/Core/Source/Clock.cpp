/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Clock.hpp"
#include <SDL3/SDL.h>

using namespace Tudo;

Clock::Clock(double framerate)
{
	mDeltaTime = 0.0;
	mLastFrame = 0.0;
	mSpeed = 1.0;
	mAccumulatedTime = 0.0;
	mFreq = SDL_GetPerformanceFrequency();
	mFixedTime = (1.0 / framerate);
	bFirstTick = true;
}

void Clock::Tick()
{
	double currentFrame = static_cast<double>(SDL_GetPerformanceCounter()) / static_cast<double>(mFreq);

	if (bFirstTick)
	{
		bFirstTick = false;
		mLastFrame = currentFrame;
	}

	mDeltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;
	mDeltaTime *= mSpeed;

	// clamp delta to avoid big spikes
	if (mDeltaTime > 0.25) mDeltaTime = 0.25; // max 250ms frame

	mAccumulatedTime += mDeltaTime;
}

bool Clock::Wait()
{
	if (mAccumulatedTime >= mFixedTime)
	{
		mAccumulatedTime -= mFixedTime;
		return true;
	}
	return false;
}

void Clock::SetSpeed(double vl)
{
	mSpeed = vl;
}

void Clock::SetFramerate(double framerate)
{
	mFixedTime = (1.0 / framerate);
}
