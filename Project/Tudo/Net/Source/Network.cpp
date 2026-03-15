/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Network.hpp"
#include "SystemTypes.hpp"
#include "BigError.hpp"
#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/ioctl.h>
#include <errno.h>
#endif

using namespace Tudo;

void Network::Initialize()
{
#if _WIN32
	WSADATA wsadata;
	int		wsaerr;

	uint16 vreq = MAKEWORD(2, 2);
	wsaerr = WSAStartup(vreq, &wsadata);
	if (wsaerr != 0)
		throw BigError("Error: Winsock DLL not found!");
#endif
}

void Network::Release()
{
#if _WIN32
	WSACleanup();
#endif
}

void Network::SetSocketNB(NetSocket& socket)
{
    ulong nb = 1;
#if _WIN32
    ioctlsocket(socket, FIONBIO, &nb);
#else
    ioctl(socket, FIONBIO, &nb);
#endif
}

void Network::CloseSocket(NetSocket& socket)
{
#if _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

int Network::GetError()
{
#if _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

void Network::Pton(sockaddr_in& service, strgv ip)
{
#if _WIN32
    InetPton(AF_INET, ip.data(), &service.sin_addr.s_addr);
#else
    inet_pton(AF_INET, ip.data(), &service.sin_addr.s_addr);
#endif
}
