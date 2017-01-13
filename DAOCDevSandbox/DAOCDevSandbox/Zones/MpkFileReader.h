#pragma once

#ifndef MPK_FILE_READER_H
#define MPK_FILE_READER_H

#include <zlib.h>

class MpkFileReader 
{

public:

	MpkFileReader();
	~MpkFileReader();
	bool init(const wchar_t*);
	char* errorString;
	bool extract(const wchar_t* path, const wchar_t* filename);
	void upload(int stage, char* data, int len);

private:
	std::string packetname_block = "";
	std::string filenames_block = "";
	int last_stage;
	std::string packet = "";
	std::vector<byte> packetname_block_vec;
	std::vector<std::string> fileNames;
	std::map<std::string, std::string> file_data;
};

#endif // MPK_FILE_READER_H
