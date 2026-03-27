/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WADMAKER_HPP_
#define WADMAKER_HPP_
#include "SystemTypes.hpp"
#include "WADFile.hpp"
#include <vector>

namespace Tudo::WADMaker
{
	struct WADMemItem
	{
		strg				Name;
		WADItem				Item;
		std::vector<uint8>	Data;
	};

	/// Generate new wad archive from existing directory structure
	void Make(strgv inpath, strgv outloc, bool saveExt = false);

	/// Create multiple new wad archives from existing directory structure
	void MakeAll(strgv fileroot, strgv outloc, bool saveExt = false);

	/// Create new wad archive from memory
	void MakeUnique(strgv filename, strgv outloc, const std::vector<WADMemItem>& data);
}
#endif