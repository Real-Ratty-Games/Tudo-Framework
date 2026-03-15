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
		~NetNode();
		virtual void Initialize(uint16 port, strgv ip) = 0;
		virtual int	 Run() = 0;

	protected:
		virtual void Release() = 0;

	protected:
        NetSocket   mSocket;
		uint16	    mPort;
		strg	    mIP;
	};
}
#endif
