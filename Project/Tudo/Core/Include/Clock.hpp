/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef CLOCK_HPP_
#define CLOCK_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	/// Managing fixed timestep
	class Clock
	{
	public:
		Clock(double framerate);
		void Tick();

		/// Use like: while(Wait())
		bool Wait();

		/// Set clock speed multiplier
		void SetSpeed(double vl);
		void SetFramerate(double framerate);

	private:
		double	mDeltaTime;
		double	mLastFrame;
		double	mSpeed;
		double	mFixedTime;
		double	mAccumulatedTime;
		uint64	mFreq;
		bool	bFirstTick;
	};
}
#endif