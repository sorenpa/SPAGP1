/*
===============================================================================
BaseCleint.h

Basic implementation of TCP/IP interface for the SPAGP1 game client.

===============================================================================
*/

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
	int Connect(PCSTR address, PCSTR port);
	int Send(char* sendBuffer, int bufferLength);
	int Recieve(char* recieveBuffer, int bufferLength);
private:
	SOCKET _socket = INVALID_SOCKET;
	int ResolvePortAndAddress(PCSTR address, PCSTR port, PADDRINFOA *addrResult);
};

