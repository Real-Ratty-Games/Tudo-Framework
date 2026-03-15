/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETDATA_HPP_
#define NETDATA_HPP_
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

#define TUDO_NET_TCP_DISCONNECTED	0
#define TUDO_NET_NOTHING			-1
#define TUDO_NET_SOCKET_INVALID		-1
#if _WIN32
#define TUDO_NET_CONNRESET			WSAECONNRESET
#else
#define TUDO_NET_CONNRESET			ECONNRESET
#endif

namespace Tudo
{
#if _WIN32
typedef SOCKET      NetSocket;
typedef TIMEVAL     NetTimeval;
typedef SOCKADDR    NetSockaddr;
#else
typedef int         NetSocket;
typedef timeval     NetTimeval;
typedef sockaddr    NetSockaddr;
#endif

	struct NetClientDataTCP
	{
        NetSocket	Socket;
		uint16		Port;
		strg		IP;
	};

	struct NetClientDataUDP
	{
		uint16		Port;
		strg		IP;
		sockaddr_in SocketAddr;
		socklen_t	SocketLen;
	};

	enum class ENetClientStatusTCP
	{
		READY,		// ready to connect
		BUSY,		// already connected
		DEAD		// connection terminated
	};
}
#endif
