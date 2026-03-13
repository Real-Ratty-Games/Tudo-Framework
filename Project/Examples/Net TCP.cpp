#include <Network.hpp>
#include <NetServerTCP.hpp>
#include <NetClientTCP.hpp>

class MyServer : public NetServerTCP
{
protected:
	int ListenToClient(const NetClientDataTCP& client)
	{
		char buffer[14];
		const int bytes = Recv(client, buffer, sizeof(buffer));

		if (bytes > 0)
		{
			buffer[bytes - 1] = '\0';
			const strg header = "Message from Client: " + client.IP;
			Window::ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, header, buffer);
		}

		if (bytes == GAMEENGINE_NET_TCP_DISCONNECTED)
			Window::ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Server Message", "One of my bois disconnected!");

		return bytes;
	}
};

class MyClient : public NetClientTCP
{
public:
	int ListenToServer()
	{
		if (Keyboard::KeyPressed(KeyboardKey::SPACE))
		{
			const char* buffer = "Hello Server! ";
			Send(buffer, strlen(buffer));
		}

		char buffer[14];
		const int bytes = Recv(buffer, sizeof(buffer));

		if (bytes == GAMEENGINE_NET_TCP_DISCONNECTED)
			Window::ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Client Message", "The big guy is gone!");

		return bytes;
	}
};

static MyServer _inst;
// static MyClient _inst;

void Initialize()
{
	Network::Initialize();
	_inst.Initialize(54000, "0.0.0.0");
}

void Tick()
{
	_inst.Run();
}

void Cleanup()
{
	_inst.Release();
	Network::Release();
}
