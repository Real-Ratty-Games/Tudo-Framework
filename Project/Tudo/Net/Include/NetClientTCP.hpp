/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETCLIENTTCP_HPP_
#define NETCLIENTTCP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetClient.hpp"

namespace Tudo
{
	class NetClientTCP : public NetClient
	{
	public:
		NetClientTCP(uint16 port, strgv ip);
		int					Run();
		ENetClientStatusTCP	Status();

	private:
		int	TryConnecting();

	protected:
		int Recv(char* buffer, int size, int flags = 0);
		int Send(const char* buffer, int size, int flags = 0);

	private:
		ENetClientStatusTCP mStatus;
	};
}
#endif