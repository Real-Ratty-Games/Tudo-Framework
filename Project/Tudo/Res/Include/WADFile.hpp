/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WADFILE_HPP_
#define WADFILE_HPP_
#include "SystemTypes.hpp"
#include <vector>
#include <unordered_map>

namespace Tudo
{
	/// Data types for wad file entry
	enum class EWADFileDataType : uint8
	{
		RAW		= 0,
		TEXTURE = 1,
		SOUND	= 2,
		MODEL	= 3
	};

	/// WAD file entry
	struct WADItem
	{
		uint				Size;
		uint				Offset;
		EWADFileDataType	Type;

		WADItem()
		{
			Size	= 0;
			Offset	= 0;
			Type	= EWADFileDataType::RAW;
		}

		WADItem(uint size, uint offset, uint8 type)
		{
			Size	= size;
			Offset	= offset;
			Type	= (EWADFileDataType)type;
		}
	};

	/// Read data from a wad file
	class WADFile
	{
	public:
		void								Open(strgv filepath);
		void								Read(strgv itemname, std::vector<uint8>& data);
		std::unordered_map<strg, WADItem>&	Items();
		void								Clear();

	private:
		strg								mFilepath;
		std::unordered_map<strg, WADItem>	mItems;
	};
}
#endif