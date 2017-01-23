// DAOCDevSandbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Networking\BaseClient.h"
#include "Zones\MpkFileReader.h"
#include "Zones\PcxConverter.h"

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
	//if (client->Connect("127.0.0.1", "10300") == 0)
	//{
	//	char * test = "This is a test";
	//	client->Send(test, (int)strlen(test));
	//}

	/**************************************************************
	* Loading of mpak data
	**************************************************************/
	MpkFileReader *reader = new MpkFileReader();

	//reader->Decompress("C:\\projects\\SPAGP1\\DAOCDevSandbox\\DAOCDevSandbox\\Zones\\Resources\\dat105.mpk");
	//reader->Decompress("C:\\Projects\\Gaming\\DAOC\\SPAGP1\\DAOCDevSandbox\\DAOCDevSandbox\\Zones\\Resources\\dat105.mpk");
	//reader->Extract("C:\\temp\\", "offset.pcx");
	//C:\Projects\Gaming\DAOC\SPAGP1\DAOCDevSandbox\DAOCDevSandbox\Zones\Resources

	/**************************************************************
	* Read .pcx data
	**************************************************************/
	PcxConverter *conver = new PcxConverter();

	std::vector<unsigned char> data;
	conver->readPCX("C:\\temp\\offset.pcx", &data);

	/*int length = 256;
	for (size_t i = 0; i < length; i++)
	{
		for (size_t j = 0; j < length; j++)
		{
			unsigned char c = 0xFF;

			if(i > 20 )
			{
				c = 0x00;
			}
				

			data.push_back(c);
		}
	}*/

	conver->writePng("C:\\temp\\offset.png", &data);
    return 0;
}

