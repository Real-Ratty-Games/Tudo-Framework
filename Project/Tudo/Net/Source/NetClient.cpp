/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClient.hpp"
#include "BigError.hpp"
#include "Network.hpp"

using namespace Tudo;

void NetClient::Initialize(uint16 port, strgv ip)
{
    mPort    = port;
    mIP      = ip;

    if (mSocket == TUDO_NET_SOCKET_INVALID)
        throw BigError("Failed creating client socket: " + std::to_string(Network::GetError()));
    
    Network::SetSocketNB(mSocket);
    Network::Pton(mService, ip);

    mService.sin_family = AF_INET;
    mService.sin_port = htons(port);
}

void NetClient::Release()
{
    Network::CloseSocket(mSocket);
}
