/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef BIGERROR_HPP_
#define BIGERROR_HPP_
#include "SystemTypes.hpp"
#include <stdexcept>

namespace Tudo
{
	class BigError : public std::runtime_error
	{
	public:
		BigError(const strg& message);
	};
}
#endif