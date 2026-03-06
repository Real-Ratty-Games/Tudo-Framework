/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/WADFile.hpp"
#include "../Include/BigError.hpp"
#include "../Include/FileSystem.hpp"
#include <fstream>
#include <sstream>

using namespace GameEngine;

void WADFile::Open(strgv filepath)
{
	if (!FileSystem::Exists(filepath))
	{
		const strg errmsg = "Cannot open WAD file, file does not exist: " + strg(filepath);
		throw BigError(errmsg);
	}
	mFilepath = filepath;

	std::ifstream file(mFilepath.data(), std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);

	uint fileCnt = 0;
	file.read(reinterpret_cast<char*>(&fileCnt), sizeof(fileCnt));

	for (uint i = 0; i < fileCnt; i++)
	{
		uint16 nameLength = 0;
		file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

		strg fileName(nameLength, '\0');
		file.read(&fileName[0], nameLength);

		uint fileSize, fileOffset;
		uint8 fileType;
		file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
		file.read(reinterpret_cast<char*>(&fileOffset), sizeof(fileOffset));
		file.read(reinterpret_cast<char*>(&fileType), sizeof(fileType));

		WADItem item(fileSize, fileOffset, fileType);
		mItems.insert(std::make_pair(fileName, item));
	}
}

void WADFile::Read(strgv itemname, std::vector<uint8>& data)
{
	const strg iname(itemname);
	if (!mItems.contains(iname))
		throw BigError("Item " + iname + " could not be found inside: " + mFilepath);

	WADItem& item = mItems[iname];

	std::ifstream file(mFilepath.data(), std::ios::binary);
	if (!file)
		throw BigError("Cannot open WAD file: " + mFilepath);

	data.resize(item.Size);

	file.seekg(static_cast<std::streampos>(item.Offset), std::ios::beg);
	if (!file.read(reinterpret_cast<char*>(data.data()), item.Size))
		throw BigError("Failed reading data here!");
}

std::unordered_map<strg, WADItem>& WADFile::Items()
{
	return mItems;
}

void WADFile::Clear()
{
	mItems.clear();
	mFilepath.clear();
}
