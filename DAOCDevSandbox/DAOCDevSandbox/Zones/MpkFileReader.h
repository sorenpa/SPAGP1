#ifndef MPK_FILE_READER_H
#define MPK_FILE_READER_H

#include <zlib.h>

class MpkFileReader 
{

public:

	MpkFileReader();
	bool init(char *);
	char* errorString;
	bool extract(const wchar_t* path,char* filename);
	void upload(int stage, char *data, int len);

private:
	char * packetname_block;
	char * filenames_block;
	int last_stage;
	char* packet;
	std::vector<std::string> fileNames;
	std::map<std::string, char[]> file_data;
};

#endif // MPK_FILE_READER_H
