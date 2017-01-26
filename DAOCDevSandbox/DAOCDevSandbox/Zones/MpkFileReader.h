/*
===============================================================================
MpKkFileReader.h

Class able to decompress .mpk archive files, and Extract the files 
contained within the archive.

This class only implements decompression for the .mpk files, since its use
is to read game data files of the original DAOC client stored in .mok archives

===============================================================================
*/
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
	bool Decompress(const char* filename);
	char* errorString;
	bool Extract(char* path, char* filename);
	std::map<std::string, std::vector<char>> GetArchiveData();
	std::string GetArchiveName();

private:
	int _lastStage;
	int _offset;
	std::vector<char> _packetBuffer;
	std::vector<char> _filenamesBuffer;
	std::vector<char> _fileDataBuffer;
	std::string _mpkName = "";
	std::vector<std::string> _fileNames;
	std::map<std::string, std::vector<char>> _fileData;

	void Upload(int stage, char* data, int len);
};

#endif // MPK_FILE_READER_H
