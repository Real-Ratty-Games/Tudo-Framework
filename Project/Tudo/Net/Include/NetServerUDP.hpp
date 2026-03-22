/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETSERVERUDP_HPP_
#define NETSERVERUDP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetServer.hpp"

namespace Tudo
{
	class NetServerUDP : public NetServer
	{
	public:
		NetServerUDP(uint16 port, strgv ip);
		~NetServerUDP();
		int Run();

	protected:
		int			Recv(NetClientDataUDP& client, char* buffer, int size, int flags = 0);
		int			Send(const NetClientDataUDP& client, const char* buffer, int size, int flags = 0);
		virtual int ListenToClient() = 0;
	};
}
#endif
