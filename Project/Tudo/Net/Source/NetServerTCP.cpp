/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetServerTCP.hpp"
#include "BigError.hpp"
#include "Network.hpp"
#include <algorithm>
#include <ranges>

using namespace Tudo;

NetServerTCP::NetServerTCP(uint16 port, strgv ip)
{
    mSocket = TUDO_NET_SOCKET_INVALID;
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	NetServer::Initialize(port, ip);
    
    if (listen(mSocket, SOMAXCONN) == TUDO_NET_SOCKET_INVALID)
		throw BigError("Failed to listen server: " + std::to_string(Network::GetError()));
}

NetServerTCP::~NetServerTCP()
{
	for (auto& client : mClients)
		Disconnect(client);
	mClients.clear();
    Network::CloseSocket(mSocket);
}

int NetServerTCP::Run()
{
	int code = Connect();
	Listen();
	return code;
}

const std::vector<NetClientDataTCP>& NetServerTCP::Clients()
{
	return mClients;
}

int NetServerTCP::Connect()
{
	sockaddr_in clientAddr;
	socklen_t	clientLen = sizeof(clientAddr);

    NetSocket client = accept(mSocket, (sockaddr*)&clientAddr, &clientLen);

    if (client == TUDO_NET_SOCKET_INVALID)
        return Network::GetError();

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));

	const uint16 cport = ntohs(clientAddr.sin_port);

	if (!std::ranges::any_of(mClients, [=](const NetClientDataTCP& c) {
		return (c.IP == ip) && (c.Port == cport);
		}))
	{
        Network::SetSocketNB(client);
		mClients.push_back({ client, cport, ip });
	}
	return TUDO_NET_NOTHING;
}

void NetServerTCP::Listen()
{
	int code = 0;
	std::erase_if(mClients, [&](auto& client)
		{
			bool remv = false;
			code = ListenToClient(client);
			if (code == TUDO_NET_TCP_DISCONNECTED) remv = true;
			else if (code == TUDO_NET_NOTHING && Network::GetError() == TUDO_NET_CONNRESET)
				remv = true;

			if (remv) Disconnect(client);
			return remv;
		});
}

int NetServerTCP::Recv(const NetClientDataTCP& client, char* buffer, int size, int flags)
{
	return recv(client.Socket, buffer, size, flags);
}

int NetServerTCP::Send(const NetClientDataTCP& client, const char* buffer, int size, int flags)
{
	return send(client.Socket, buffer, size, flags);
}

void NetServerTCP::Disconnect(NetClientDataTCP& client)
{
    Network::CloseSocket(client.Socket);
}
