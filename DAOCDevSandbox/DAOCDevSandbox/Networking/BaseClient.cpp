
#include "../stdafx.h"
#include "BaseClient.h"

#define DEFAULT_BUFLEN 512

BaseClient::BaseClient()
{
}

BaseClient::~BaseClient() 
{
	// cleanup
	closesocket(_socket);
	WSACleanup();
}

int BaseClient::connect(PCSTR address, PCSTR port)
{
	int iResult;

	struct addrinfo *addrInfoResult = NULL;
	resolvePortAndAddress(address, port, &addrInfoResult);

	addrinfo *ptr = NULL;
	// Attempt to connect to an address until one succeeds
	for (ptr = addrInfoResult; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		_socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			printf("[BaseClient.cpp]:socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = ::connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(_socket);
			_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(addrInfoResult);

	if (_socket == INVALID_SOCKET) {
		printf("[BaseClient.cpp]:Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	return 0;
}

int BaseClient::send(char * sendBuffer, int bufferLength)
{
	// Send an initial buffer
	int iResult = ::send(_socket, sendBuffer, bufferLength, 0);
	if (iResult == SOCKET_ERROR) {
		printf("[BaseClient.cpp]:send failed with error: %d\n", WSAGetLastError());
		closesocket(_socket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	return 0;
}

int BaseClient::recieve(char* recieveBuffer, int bufferLength)
{
	
	int iResult = 0;
	// Receive until the peer closes the connection
	do {

		int iResult = recv(_socket, recieveBuffer, bufferLength, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("[BaseClient.cpp]:Connection closed\n");
		else
			printf("[BaseClient.cpp]:recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	return 0;
}

int BaseClient::resolvePortAndAddress(PCSTR address, PCSTR port, PADDRINFOA *addrResult)
{
	int iResult;
	//struct addrinfo  * hints;
	
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(address, port, &hints, addrResult);
	if (iResult != 0) {
		printf("[BaseClient.cpp]:getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	return 0;
}



