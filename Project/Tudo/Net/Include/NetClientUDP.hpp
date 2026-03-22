/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETCLIENTUDP_HPP_
#define NETCLIENTUDP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetClient.hpp"

namespace Tudo
{
	class NetClientUDP : public NetClient
	{
	public:
		NetClientUDP(uint16 port, strgv ip);
		int Run();

	protected:
		int Recv(char* buffer, int size, int flags = 0);
		int Send(const char* buffer, int size, int flags = 0);

	private:
		socklen_t mServiceLen;
	};
}
#endif