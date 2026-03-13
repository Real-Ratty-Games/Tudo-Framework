#include <Network.hpp>
#include <NetServerUDP.hpp>
#include <NetClientUDP.hpp>

class MyServer : public NetServerUDP
{
protected:
	int ListenToClient()
	{
		NetClientDataUDP client;

		char buffer[14];
		const int bytes = Recv(client, buffer, sizeof(buffer));

		if (bytes != GAMEENGINE_NET_NOTHING)
		{
			buffer[bytes - 1] = '\0';
			const strg header = client.IP;
			Window::ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, header, buffer);
		}
		return bytes;
	}
};

class MyClient : public NetClientUDP
{
public:
	int ListenToServer()
	{
		if (Keyboard::KeyPressed(KeyboardKey::SPACE))
		{
			const char* buffer = "Hello Server! ";
			Send(buffer, strlen(buffer));
		}

		return GAMEENGINE_NET_NOTHING;
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
