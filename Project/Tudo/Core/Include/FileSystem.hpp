/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_
#include "SystemTypes.hpp"
#include <filesystem>
#include <vector>

/// <summary>
/// Functions for managing system files and directories
/// </summary>
namespace Tudo::FileSystem
{
	/// <summary>
	/// Returns true if File/Path exists
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns></returns>
	bool					Exists(strgv filepath);
	void					FileRemove(strgv filepath);

	/// <summary>
	/// 0 = full
	/// 1 = name
	/// 2 = extension
	/// </summary>
	/// <param name="dir"></param>
	/// <param name="nt"></param>
	/// <returns></returns>
	std::vector<strg>		FilesInDirectory(strgv dir, int nt);
	std::vector<strg>		SubDirectories(strgv dir);
	void					DirectoryCreate(strgv dir);
	void					DirectoryRemove(strgv dir);
	std::vector<char>		ReadBinaryFile(strgv filepath);
	void					WriteBinaryFile(strgv filepath, std::vector<char>& data);
	void					WriteTextFile(strgv filepath, strgv text);

	std::filesystem::path	GetResourcePath(strgv filename);
}
#endif
