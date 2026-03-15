/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "FileSystem.hpp"
#include <filesystem>
#include <fstream>

#if __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace Tudo;

bool FileSystem::Exists(strgv filepath)
{
	return std::filesystem::exists(std::filesystem::path(filepath));
}

void FileSystem::FileRemove(strgv filepath)
{
	std::filesystem::remove(filepath);
}

std::vector<strg> FileSystem::FilesInDirectory(strgv dir, int nt)
{
	if (Exists(dir))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if (entry.is_regular_file())
			{
				strg pb = entry.path().string();
				switch (nt)
				{
				case 1: { pb = entry.path().stem().string(); }break;
				case 2: { pb = entry.path().extension().string(); }break;
				}
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

std::vector<strg> FileSystem::SubDirectories(strgv dir)
{
	if (Exists(dir))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if (entry.is_directory())
			{
				strg pb = entry.path().string();
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

void FileSystem::DirectoryCreate(strgv dir)
{
	std::filesystem::create_directory(dir);
}

void FileSystem::DirectoryRemove(strgv dir)
{
	std::filesystem::remove_all(dir);
}

std::vector<char> FileSystem::ReadBinaryFile(strgv filepath)
{
	std::vector<char> rtVl;

	std::ifstream File(filepath.data(), std::ios::in | std::ios::binary | std::ios::ate);
	std::streamsize size = File.tellg();
	rtVl.resize(size);

	File.seekg(0, std::ios::beg);
	File.read(rtVl.data(), size);
	File.close();

	return rtVl;
}

void FileSystem::WriteBinaryFile(strgv filepath, std::vector<char>& data)
{
	std::ofstream file(filepath.data(), std::ios::binary);
	file.write(data.data(), data.size());
	file.close();
}

void FileSystem::WriteTextFile(strgv filepath, strgv text)
{
	std::ofstream file(filepath.data());
	file.write(text.data(), text.size());
	file.close();
}

std::filesystem::path FileSystem::GetResourcePath(strgv filename)
{
#if __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (CFURLGetFileSystemRepresentation(resourcesURL, true, (uint8*)path, PATH_MAX)) {
            CFRelease(resourcesURL);
            return std::filesystem::path(path) / filename;
        }
        CFRelease(resourcesURL);
#endif
    return filename;
}
