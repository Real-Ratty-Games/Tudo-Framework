/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SPRITEANIMATION_HPP_
#define SPRITEANIMATION_HPP_
#include "SystemTypes.hpp"
#include "Alarm.hpp"

namespace Tudo
{
	/// Sprite Animation types
	enum class ESpriteAnimationStyle
	{
		NONE,			// 0 -> 1 -> 2 -> x
		LOOP,			// 0 -> 1 -> 2 -> 0 -> ..
		REVERSE,		// 0 -> 1 -> 2 -> 1 -> 0 -> x
		REVERSELOOP		// 0 -> 1 -> 2 -> 1 -> 0 -> 1 -> ..
	};

	/// Sprite animation properties
	struct SpriteAnimation
	{
		vec2					FrameSize;
		vec2					StartIndex;
		int64					Speed;				// in frames per second
		int						RowCount;			// how many frames in one row
		int						TotalFrameCount;	// how many frames in total
		ESpriteAnimationStyle	Style = ESpriteAnimationStyle::NONE;
	};

	/// Manages sprite animation
	class SpriteAnimator
	{
	public:
		SpriteAnimator();
		void Play(SpriteAnimation* animation);
		void Replay();
		void Stop();
		void Pause(bool paused);

		/// Returns true if animation has finished
		/// Does not check while looping
		bool IsFinished();
		void Tick();

		SpriteAnimation*	GetAnimation();
		vec2				GetCurrentIndex();

	private:
		int64 AdvanceFrame();

	private:
		SpriteAnimation*	pAnimation;

		Alarm				mAlarm;

		int					mCurrentFrame;
		vec2				mCurrentIndex;
		int					mFrameMul;
		bool				bAnimationFinished;
	};
}
#endif