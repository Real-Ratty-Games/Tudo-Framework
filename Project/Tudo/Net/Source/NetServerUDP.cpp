/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetServerUDP.hpp"
#include "BigError.hpp"
#include "Network.hpp"

using namespace Tudo;

NetServerUDP::NetServerUDP(uint16 port, strgv ip)
{
    mSocket = TUDO_NET_SOCKET_INVALID;
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	NetServer::Initialize(port, ip);
}

NetServerUDP::~NetServerUDP()
{
    Network::CloseSocket(mSocket);
}

int NetServerUDP::Run()
{
	return ListenToClient();
}

int NetServerUDP::Recv(NetClientDataUDP& client, char* buffer, int size, int flags)
{
	sockaddr_in clientAddr;
	socklen_t	clientLen = sizeof(clientAddr);

    int bytes = recvfrom(mSocket, buffer, size, flags, (NetSockaddr*)&clientAddr, &clientLen);

    if (bytes == TUDO_NET_SOCKET_INVALID)
		return TUDO_NET_NOTHING;

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));

	const uint16 cport = ntohs(clientAddr.sin_port);

	client.IP			= ip;
	client.Port			= cport;
	client.SocketAddr	= clientAddr;
	client.SocketLen	= clientLen;

	return bytes;
}

int NetServerUDP::Send(const NetClientDataUDP& client, const char* buffer, int size, int flags)
{
    return sendto(mSocket, buffer, size, flags, (NetSockaddr*)&client.SocketAddr, client.SocketLen);
}
