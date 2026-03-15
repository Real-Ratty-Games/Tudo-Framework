/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETSERVER_HPP_
#define NETSERVER_HPP_
#include "SystemTypes.hpp"
#include "NetNode.hpp"

namespace Tudo
{
	class NetServer : public NetNode
	{
	public:
		void Initialize(uint16 port, strgv ip);
	};
}
#endif