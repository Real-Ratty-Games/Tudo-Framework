/*======================================================
    Copyright (c) 2026 Real Ratty Games.
    Created by Norbert Gerberg.
======================================================*/
#include "IniFile.hpp"
#include <fstream>
#include <sstream>

using namespace Tudo;

bool IniFile::Open(strgv filename)
{
    mFilepath = filename;
    mMemBuffer = nullptr;

    std::fstream file(mFilepath);
    if (!file) return false;

    std::ostringstream oss;
    oss << file.rdbuf();
    ReadString(oss.str());
    return true;
}

void IniFile::OpenMem(std::vector<uint8>* mem)
{
    mMemBuffer = mem;
    strg content(reinterpret_cast<const char*>(mem->data()), mem->size());
    ReadString(content);
}

strg IniFile::Read(strgv section, strgv key, strgv value)
{
    if (auto sec = mBuffer.find(section.data()); sec != mBuffer.end())
    {
        if (auto it = sec->second.find(key.data()); it != sec->second.end())
            return it->second;
    }
    return strg(value);
}

void IniFile::Write(strgv section, strgv key, strgv value)
{
    mBuffer[section.data()][key.data()] = value;
}

void IniFile::CloseMem()
{
    strg content = WriteString();
    mMemBuffer->assign(content.begin(), content.end());
    mMemBuffer = nullptr;
}

void IniFile::Close()
{
    std::ofstream file(mFilepath);
    if (!file) return;
    file << WriteString();
    file.close();
}

strg IniFile::Trim(const strg& s)
{
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    return (start == strg::npos) ? "" : s.substr(start, end - start + 1);
}

void IniFile::ReadString(const strg& content)
{
    mBuffer.clear();
    std::istringstream stream(content);
    strg line, current_section;

    while (std::getline(stream, line))
    {
        auto comment_pos = line.find_first_of(";#");
        if (comment_pos != strg::npos)
            line = line.substr(0, comment_pos);

        line = Trim(line);
        if (line.empty()) continue;

        if (line.front() == '[' && line.back() == ']')
            current_section = line.substr(1, line.length() - 2);
        else
        {
            auto delim_pos = line.find('=');
            if (delim_pos != strg::npos)
            {
                strg key = Trim(line.substr(0, delim_pos));
                strg value = Trim(line.substr(delim_pos + 1));
                mBuffer[current_section][key] = value;
            }
        }
    }
}

strg IniFile::WriteString() const
{
    std::ostringstream stream;

    for (const auto& [section, kv] : mBuffer)
    {
        stream << "[" << section << "]\n";
        for (const auto& [key, value] : kv)
            stream << key << "=" << value << "\n";
        stream << "\n";
    }
    return stream.str();
}
