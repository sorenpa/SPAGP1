#pragma once

#ifndef MPK_FILE_READER_H
#define MPK_FILE_READER_H

#include <zlib.h>
#include <iostream>
#include <fstream>

class MpkFileReader 
{

public:

	MpkFileReader();
	~MpkFileReader();
	bool init(const char*);
	char* errorString;
	bool extract(char* path, char* filename);
	void upload(int stage, char* data, int len);

private:
	int _lastStage;
	int _offset;
	char* _tmpPtr;
	std::vector<char> _packetBuffer;
	std::vector<char> _filenamesBuffer;
	std::vector<char> _fileDataBuffer;
	std::string _mpkName = "";
	std::vector<std::string> _fileNames;
	std::map<std::string, std::vector<char>> _fileData;
};

#endif // MPK_FILE_READER_H
