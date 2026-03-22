/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ALARM_HPP_
#define ALARM_HPP_
#include "SystemTypes.hpp"
#include <functional>
#include <unordered_map>
#include <unordered_set>

#define TUDO_ALARM_DISABLE 0
#define TUDO_ALARM_BIND(F) std::bind(&F, this)

namespace Tudo
{
	/// Alarm module properties
	struct AlarmModule
	{
		std::function<int64()>	Method;
        int64					Count = 0;
		bool					bPaused = false;
	};

	/// Alarm system for timed logic
	class Alarm
	{
	public:
		void PushModule(strgv name, std::function<int64()> method, int64 count);
		void SetModuleCount(strgv name, int64 count);
		void SetModulePaused(strgv name, bool paused);
		bool IsModuleActive(strgv name);
		void Tick();

	private:
		std::unordered_map<strg, AlarmModule>	mModules;
		std::unordered_set<strg>				mActiveModules;
	};
}
#endif
