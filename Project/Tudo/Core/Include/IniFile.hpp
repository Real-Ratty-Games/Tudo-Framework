/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef INIFILE_HPP_
#define INIFILE_HPP_
#include "SystemTypes.hpp"
#include <vector>
#include <unordered_map>

namespace Tudo
{
	class IniFile
	{
	public:
		using Section = std::unordered_map<strg, strg>;
		using Content = std::unordered_map<strg, Section>;

		bool Open(strgv filename);
		void OpenMem(std::vector<uint8>& mem);
		strg Read(strgv section, strgv key, strgv value);
		void Write(strgv section, strgv key, strgv value);
		void CloseMem();
		void Close();

	private:
		strg Trim(const strg& s);
		void ReadString(const strg& content);
		strg WriteString() const;

	private:
		strg				mFilepath;
		Content				mBuffer;
		std::vector<uint8>* mMemBuffer;
	};
}
#endif
