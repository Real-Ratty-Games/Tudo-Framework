/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Memory.hpp"
#include "BigError.hpp"

using namespace Tudo;

void Memory::Copy(void* dst, const void* src, uint64 size)
{
	if (!dst || !src || size == 0)
		throw BigError("Memory is invalid!");
	std::memcpy(dst, src, size);
}
