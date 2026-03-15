/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Alarm.hpp"

using namespace Tudo;

void Alarm::PushModule(strgv name, std::function<int64()> method, int64 count)
{
	strg sname(name);
	if (!mModules.contains(sname))
	{
		AlarmModule result;
		result.Method = method;
		result.Count = count;
		mModules.insert(std::make_pair(sname, result));

		if (count > TUDO_ALARM_DISABLE && !mActiveModules.contains(sname))
			mActiveModules.insert(sname);
	}
}

void Alarm::SetModuleCount(strgv name, int64 count)
{
	strg sname(name);
	if (mModules.contains(sname))
	{
		mModules[sname].Count = count;

		if (count > TUDO_ALARM_DISABLE && !mActiveModules.contains(sname))
			mActiveModules.insert(sname);
		else if(count <= TUDO_ALARM_DISABLE)
			mActiveModules.erase(sname);
	}
}

void Alarm::SetModulePaused(strgv name, bool paused)
{
	strg sname(name);
	if (mModules.contains(sname))
		mModules[sname].bPaused = paused;
}

bool Alarm::IsModuleActive(strgv name)
{
	strg sname(name);
	if (mModules.contains(sname))
		return mActiveModules.contains(sname) && !mModules[sname].bPaused;
	return false;
}

void Alarm::Tick()
{
	std::vector<strg> toRemove;
	toRemove.reserve(mActiveModules.size());

	for (auto& it : mActiveModules)
	{
		auto& module = mModules[it];
		if (module.bPaused) continue;

		module.Count--;
		if (module.Count <= TUDO_ALARM_DISABLE)
		{
			module.Count = module.Method();

			if (module.Count <= TUDO_ALARM_DISABLE)
				toRemove.push_back(it);
		}
	}

	if (!toRemove.empty())
	{
		for (auto& it : toRemove)
			mActiveModules.erase(it);
	}
}
