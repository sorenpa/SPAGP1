#pragma once

#ifndef MPK_FILE_READER_H
#define MPK_FILE_READER_H

#include <zlib.h>

class MpkFileReader 
{

public:

	MpkFileReader();
	~MpkFileReader();
	bool init(const char*);
	char* errorString;
	bool extract(const char* path, const char* filename);
	void upload(int stage, char* data, int len);

private:
	int _lastStage;
	std::string _mpkName = "";
	std::string _filenamesBlock = "";
	std::vector<std::string> _fileNames;
	std::map<std::string, std::string> _fileData;
};

#endif // MPK_FILE_READER_H
