/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClientUDP.hpp"
#include "BigError.hpp"

using namespace Tudo;

NetClientUDP::NetClientUDP(uint16 port, strgv ip)
{
    mSocket = TUDO_NET_SOCKET_INVALID;
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	NetClient::Initialize(port, ip);
	mServiceLen = sizeof(mService);
}

int NetClientUDP::Run()
{
	return ListenToServer();
}

int NetClientUDP::Recv(char* buffer, int size, int flags)
{
	return recvfrom(mSocket, buffer, size, flags, (NetSockaddr*)&mService, &mServiceLen);
}

int NetClientUDP::Send(const char* buffer, int size, int flags)
{
    return sendto(mSocket, buffer, size, flags, (NetSockaddr*)&mService, mServiceLen);
}
