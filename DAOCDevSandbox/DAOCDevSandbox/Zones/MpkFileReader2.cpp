
#include "stdafx.h"
#include "MpkFileReader.h"

using namespace std;


MpkFileReader::MpkFileReader()
{
}

MpkFileReader::~MpkFileReader()
{
}

int MpkFileReader::open(char *path)
{
	ifstream inputStream;

	inputStream.open(path, ios::in | ios::ate);
	
	if (!inputStream.is_open()) 
	{
		cerr << "[MpkFileReader.cpp]: Could not open file " << path;
		return 1;
	}

	char line[256];
	while (!inputStream.getline(line, 256))
	{
		cout << line << '\n';
	}

	inputStream.close();

	return 0;
}

int MpkFileReader::read()
{
	return 0;
}

int MpkFileReader::uncompress()
{
	return 0;
}


