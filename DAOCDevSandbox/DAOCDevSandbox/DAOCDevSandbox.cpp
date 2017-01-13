// DAOCDevSandbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Networking\BaseClient.h"
#include "Zones\MpkFileReader.h"

int main()
{

	/**************************************************************
	* Basick networking client
	**************************************************************/

	//WSADATA wsaData;

	//// Initialize Winsock
	//int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0) {
	//	printf("WSAStartup failed with error: %d\n", iResult);
	//	return 1;
	//}

	//BaseClient * client = new BaseClient();
	//if (client->connect("127.0.0.1", "10300") == 0)
	//{
	//	char * test = "This is a test";
	//	client->send(test, (int)strlen(test));
	//}

	/**************************************************************
	* Loading of mpak data
	**************************************************************/
	MpkFileReader *reader = new MpkFileReader();
	

	
	reader->init(L"C:\\projects\\SPAGP1\\DAOCDevSandbox\\DAOCDevSandbox\\Zones\\Resources\\dat105.mpk");




    return 0;
}

