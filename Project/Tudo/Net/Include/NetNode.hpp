/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETNODE_HPP_
#define NETNODE_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"

namespace Tudo
{
	class NetNode
	{
	public:
		virtual int Run() = 0;

	protected:
        NetSocket   mSocket;
		uint16	    mPort;
		strg	    mIP;
	};
}
#endif
