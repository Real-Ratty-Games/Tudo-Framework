/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_
#include "SystemTypes.hpp"
#include <filesystem>
#include <vector>

namespace Tudo::FileSystem
{
	bool					Exists(strgv filepath);
	void					FileRemove(strgv filepath);

	/// 0 = full
	/// 1 = name
	/// 2 = extension
	std::vector<strg>		FilesInDirectory(strgv dir, int nt);
	std::vector<strg>		SubDirectories(strgv dir);
	void					DirectoryCreate(strgv dir);
	void					DirectoryRemove(strgv dir);
	std::vector<char>		ReadBinaryFile(strgv filepath);
	void					WriteBinaryFile(strgv filepath, const std::vector<char>& data);
	void					WriteTextFile(strgv filepath, strgv text);

	std::filesystem::path	GetResourcePath(strgv filename);
}
#endif
