/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETCLIENT_HPP_
#define NETCLIENT_HPP_
#include "SystemTypes.hpp"
#include "NetNode.hpp"

namespace Tudo
{
	class NetClient : public NetNode
	{
	public:
		~NetClient();

	protected:
		void Initialize(uint16 port, strgv ip);

		virtual int Recv(char* buffer, int size, int flags = 0) = 0;
		virtual int Send(const char* buffer, int size, int flags = 0) = 0;
		virtual int ListenToServer() = 0;

	protected:
		sockaddr_in mService;
	};
}
#endif