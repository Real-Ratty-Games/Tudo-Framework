/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef CLOCK_HPP_
#define CLOCK_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	/// <summary>
	/// Managing fixed timestep
	/// </summary>
	class Clock
	{
	public:
		Clock(double framerate);
		void Tick();

		/// <summary>
		/// Use like: while(Wait())
		/// </summary>
		/// <returns>True if frame passed</returns>
		bool Wait();

		/// <summary>
		/// Set clock speed multiplier
		/// </summary>
		/// <param name="vl"></param>
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