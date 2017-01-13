#pragma once

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class BaseClient
{
public:
	BaseClient();
	~BaseClient();
	int connect(PCSTR address, PCSTR port);
	int send(char* sendBuffer, int bufferLength);
	int recieve(char* recieveBuffer, int bufferLength);
private:
	SOCKET _socket = INVALID_SOCKET;
	int resolvePortAndAddress(PCSTR address, PCSTR port, PADDRINFOA *addrResult);
};

