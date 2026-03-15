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

	/// <summary>
	/// Generate new wad archive from existing directory structure
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="outloc"></param>
	/// <param name="saveExt"></param>
	void Make(strgv inpath, strgv outloc, bool saveExt = false);

	/// <summary>
	/// Create multiple new wad archives from existing directory structure
	/// </summary>
	/// <param name="fileroot"></param>
	/// <param name="outloc"></param>
	/// <param name="saveExt"></param>
	void MakeAll(strgv fileroot, strgv outloc, bool saveExt = false);

	/// <summary>
	/// Create new wad archive from memory
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="outloc"></param>
	/// <param name="data"></param>
	void MakeUnique(strgv filename, strgv outloc, std::vector<WADMemItem>& data);
}
#endif