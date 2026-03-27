/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "SpriteAnimation.hpp"
#include "Sprite.hpp"

using namespace Tudo;

SpriteAnimator::SpriteAnimator()
{
	pAnimation = nullptr;
	mCurrentFrame = 0;
	mCurrentIndex = vec2(0);
	mFrameMul = 1;
	bAnimationFinished = false;
	mAlarm.PushModule("AdvanceFrame", TUDO_ALARM_BIND(SpriteAnimator::AdvanceFrame), TUDO_ALARM_DISABLE);
}

void SpriteAnimator::Play(SpriteAnimation* animation)
{
	pAnimation = animation;
	Replay();
}

void SpriteAnimator::Replay()
{
	mCurrentFrame = 0;
	mFrameMul = 1;
	mCurrentIndex = pAnimation->StartIndex;
	bAnimationFinished = false;
	Pause(false);
	mAlarm.SetModuleCount("AdvanceFrame", pAnimation->Speed);
}

void SpriteAnimator::Stop()
{
	mAlarm.SetModuleCount("AdvanceFrame", TUDO_ALARM_DISABLE);
}

void SpriteAnimator::Pause(bool paused)
{
	mAlarm.SetModulePaused("AdvanceFrame", paused);
}

bool SpriteAnimator::IsFinished()
{
	return bAnimationFinished;
}

void SpriteAnimator::Tick()
{
	mAlarm.Tick();
}

SpriteAnimation* SpriteAnimator::GetAnimation() const
{
	return pAnimation;
}

vec2 SpriteAnimator::GetCurrentIndex() const
{
	return mCurrentIndex;
}

int64 SpriteAnimator::AdvanceFrame()
{
	const int	totalframes = pAnimation->TotalFrameCount - 1;
	const float maxrow		= (pAnimation->StartIndex.X + pAnimation->RowCount - 1);

	mCurrentFrame += mFrameMul;
	if (mFrameMul == 1 && mCurrentFrame > totalframes)
	{
		switch (pAnimation->Style)
		{
		case ESpriteAnimationStyle::NONE:
		{
			mCurrentFrame = totalframes;
			bAnimationFinished = true;
			return TUDO_ALARM_DISABLE;
		}break;
		case ESpriteAnimationStyle::LOOP:
		{
			mCurrentFrame = 0;
			mCurrentIndex = pAnimation->StartIndex;
			return pAnimation->Speed;
		}
		default:
		{
			mCurrentFrame	= totalframes;
			mFrameMul		= -1;
		}break;
		}
	}

	if (mFrameMul == -1 && mCurrentFrame <= 0)
	{
		if (pAnimation->Style == ESpriteAnimationStyle::REVERSELOOP)
		{
			mFrameMul = 1;
			mCurrentFrame += mFrameMul;

			mCurrentIndex.X += mFrameMul;
			if (mCurrentIndex.X > maxrow)
			{
				mCurrentIndex.X = pAnimation->StartIndex.X;
				mCurrentIndex.Y++;
			}
			return pAnimation->Speed;
		}
		else
		{
			mCurrentFrame = 0;
			bAnimationFinished = true;
			return TUDO_ALARM_DISABLE;
		}
	}

	mCurrentIndex.X += mFrameMul;
	if (mFrameMul == 1 && mCurrentIndex.X > maxrow)
	{
		mCurrentIndex.X = pAnimation->StartIndex.X;
		mCurrentIndex.Y++;
	}
	else if (mCurrentIndex.X < pAnimation->StartIndex.X)
	{
		mCurrentIndex.X = maxrow;
		mCurrentIndex.Y--;
	}
	return pAnimation->Speed;
}
