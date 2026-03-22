/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "WADMaker.hpp"
#include "IniFile.hpp"
#include "FileSystem.hpp"
#include <filesystem>
#include <fstream>

using namespace Tudo;

static std::vector<strg> WADMaker_GetFilesinDir(strgv filepath, bool on = false, bool saveExt = false);
static std::vector<uint8> WADMaker_GetFiletypesinDir(strgv filepath);

void WADMaker::Make(strgv inpath, strgv outloc, bool saveExt)
{
	const strg soutloc(outloc);
	strg infoFilepath(inpath);
	infoFilepath += "info.ini";

	if (!FileSystem::Exists(infoFilepath))
		return;

	IniFile ini;
	ini.Open(infoFilepath);
	strg outname = ini.Read("default", "name", "UNKNOWN");
	outname += "." + ini.Read("default", "extension", "wad");
	ini.Close();

	std::vector<strg> files = WADMaker_GetFilesinDir(inpath);
	std::vector<strg> filenames = WADMaker_GetFilesinDir(inpath, true, saveExt);
	std::vector<uint8> filetypes = WADMaker_GetFiletypesinDir(inpath);
	std::vector<std::vector<uint8>> data;

	for (auto& it : files)
	{
		if (!FileSystem::Exists(it))
			return;

		std::ifstream file(it, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8> buffer(size);
		if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
			return;

		file.close();
		data.push_back(buffer);
	}

	std::ofstream OUT(soutloc + outname, std::ios::binary);

	uint fileCnt = static_cast<uint>(filenames.size());
	OUT.write(reinterpret_cast<const char*>(&fileCnt), sizeof(fileCnt));

	uint currentOffset = sizeof(fileCnt);

	for (uint i = 0; i < fileCnt; ++i)
	{
		currentOffset += sizeof(uint16);
		currentOffset += static_cast<uint>(filenames[i].size());
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint8);
	}

	for (uint i = 0; i < fileCnt; ++i)
	{
		uint16 nameLength = static_cast<uint>(filenames[i].size());
		uint fileSize = static_cast<uint>(data[i].size());
		uint fileOffset = currentOffset;
		uint8 fileType = filetypes[i];

		OUT.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
		OUT.write(filenames[i].data(), nameLength);
		OUT.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
		OUT.write(reinterpret_cast<const char*>(&fileOffset), sizeof(fileOffset));
		OUT.write(reinterpret_cast<const char*>(&fileType), sizeof(fileType));

		currentOffset += fileSize;
	}

	for (auto& it : data)
		OUT.write(reinterpret_cast<const char*>(it.data()), it.size());

	OUT.close();
}

void WADMaker::MakeAll(strgv fileroot, strgv outloc, bool saveExt)
{
	std::vector<strg> totalpaths;
	for (auto& it : std::filesystem::recursive_directory_iterator(fileroot))
	{
		if (it.is_regular_file() && it.path().filename() == "info.ini")
			totalpaths.push_back(it.path().parent_path().string() + "\\");
	}

	for (auto& it : totalpaths) Make(it, outloc, saveExt);
}

void WADMaker::MakeUnique(strgv filename, strgv outloc, std::vector<WADMemItem>& data)
{
	const strg soutloc(outloc);
	strg OUTPATH = soutloc + strg(filename);
	std::ofstream OUT(OUTPATH.data(), std::ios::binary);

	uint fileCnt = static_cast<uint>(data.size());
	OUT.write(reinterpret_cast<const char*>(&fileCnt), sizeof(fileCnt));

	uint currentOffset = sizeof(fileCnt);

	for (auto& it : data)
	{
		currentOffset += sizeof(uint16);
		currentOffset += static_cast<uint>(it.Name.size());
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint8);
	}

	for (auto& it : data)
	{
		uint16 nameLength = static_cast<uint16>(it.Name.size());
		uint fileSize = static_cast<uint>(it.Data.size());
		uint fileOffset = currentOffset;
		uint8 fileType = (uint8)it.Item.Type;

		OUT.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
		OUT.write(it.Name.data(), nameLength);
		OUT.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
		OUT.write(reinterpret_cast<const char*>(&fileOffset), sizeof(fileOffset));
		OUT.write(reinterpret_cast<const char*>(&fileType), sizeof(fileType));

		currentOffset += fileSize;
	}

	for (auto& it : data)
		OUT.write(reinterpret_cast<const char*>(it.Data.data()), it.Data.size());

	OUT.close();
}

/*======================================================
======================================================*/

/// Returns list of files in a directory that are of valid structure
std::vector<strg> WADMaker_GetFilesinDir(strgv filepath, bool on, bool saveExt)
{
	if (std::filesystem::exists(filepath))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(filepath))
		{
			if (entry.is_regular_file())
			{
				strg pb = entry.path().string();

				if (pb.find("info.ini") != strg::npos) continue;

				if (on) pb = entry.path().stem().string();
				if (saveExt) pb = entry.path().filename().string();
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

/// Returns list of file types in a directory that are of valid structure
std::vector<uint8> WADMaker_GetFiletypesinDir(strgv filepath)
{
	std::vector<strg> image_ext = {
		"png", "PNG", "Png",
		"jpg", "JPG", "Jpg",
		"bmp", "BMP", "Bmp",
		"jpeg", "JPEG", "Jpeg",
		"dds", "DDS", "Dds",
		"gif", "GIF", "Gif",
		"tga", "TGA", "Tga"
		"ktx", "KTX", "Ktx"
		"ktx2", "KTX2", "Ktx2"
		"pvr", "PVR", "Pvr"
	};

	std::vector<strg> sound_ext = {
		"wav", "WAV", "Wav",
		"mp3", "MP3", "Mp3",
		"aif", "AIF", "Aif",
		"mid", "MID", "Mid",
		"flac", "FLAC", "Flac",
		"m4a", "M4A", "M4a",
		"mp4", "MP4", "Mp4",
		"wma", "WMA", "Wma",
		"aac", "AAC", "Aac"
	};

	std::vector<strg> model_ext = {
		"fbx", "FBX", "Fbx",
		"obj", "OBJ", "Obj",
		"mtl", "MTL", "Mtl",
		"3d", "3D",
		"3ds", "3DS",
		"dae", "DAE", "Dae",
		"gltf", "GLTF", "Gltf",
		"glb", "GLB", "Glb",
		"m3d", "M3D", "M3d"
	};

	if (std::filesystem::exists(filepath))
	{
		std::vector<uint8> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(filepath))
		{
			if (entry.is_regular_file())
			{
				if (entry.path().string().find("info.ini") != strg::npos) continue;

				strg pb = entry.path().extension().string();
				pb = pb.erase(0, 1);

				uint8 vl = 0;
				if (std::find(image_ext.begin(), image_ext.end(), pb) != image_ext.end()) vl = 1;
				else if (std::find(sound_ext.begin(), sound_ext.end(), pb) != sound_ext.end()) vl = 2;
				else if (std::find(model_ext.begin(), model_ext.end(), pb) != model_ext.end()) vl = 3;
				else vl = 0;

				rtVl.push_back(vl);
			}
		}
		return rtVl;
	}
	return {};
}
