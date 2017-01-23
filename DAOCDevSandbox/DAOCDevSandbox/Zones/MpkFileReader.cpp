#include "../stdafx.h"
#include "MpkFileReader.h"

/*
====================
MpkFileReader

	Constructor
====================
*/
MpkFileReader::MpkFileReader() 
{
	_lastStage = -1;
}

/*
====================
~MpkFileReader

Destructor
====================
*/
MpkFileReader::~MpkFileReader()
{
}

/*
=====================================================================
Decompress

	Decompresses the .mpk file specified by 'file'. The results are
	stored in the private field _fileData.
=====================================================================
*/
bool MpkFileReader::Decompress(const char* file) {
	_offset = 0;

	int inlen = 0;
	int stage = 0;
	char inbuf[1024],outbuf[1024];
	z_stream stream;
	
	FILE *fp;

	errno_t err;
	if ((err = fopen_s(&fp, file, "rb")) != 0) {
		errorString = "Error opening file";
		return false;
	}

	// read and check header
	fseek(fp, 0, SEEK_SET);
	fread (inbuf,1,4,fp);
	inbuf[4]='\0';
	if ( strcmp("MPAK",inbuf) ) {
		errorString="Not a valid MPAK file";
		return false;	
	}
	// zstream start
	fseek(fp, 21, SEEK_SET);
	
	memset(&stream,0,sizeof(z_stream));

	inflateInit(&stream); 
	while (!feof(fp) || inlen > 0) {
		int rc=0;

		/* .. read some data */
		inlen += fread(inbuf + inlen, 1, sizeof(inbuf) - inlen, fp);

		/* .. decompress it */
		stream.next_in = (unsigned char *)inbuf;
		stream.avail_in = inlen;
		
		stream.next_out = (unsigned char *)outbuf;
		stream.avail_out = sizeof(outbuf);

		rc = inflate(&stream, 0);
		if (rc != Z_STREAM_END && rc != Z_OK) {
			/* decompression error */
			errorString = "inflate returned"; 
			return false;
		}

		/* if we have some decompressed data, process it */
        if ((char*)stream.next_out > outbuf)
			Upload(stage, outbuf, (char*)stream.next_out - outbuf);

		/* if zlib consumed some data, rearrange our buffers */
		if ((char*)stream.next_in > inbuf) {
			memmove(inbuf, stream.next_in, stream.avail_in);
			inlen = stream.avail_in;
		}
		if (rc == Z_STREAM_END) {
			/* end of compression stream, reinit the decompressor for the */
			/* next stream */
			++stage;
			inflateEnd(&stream);
			inflateInit(&stream);
		}
    }

	inflateEnd(&stream);
	fclose(fp);
	return true;
}

/*
=====================================================================
Decompress

	Extracts the archived file specified by 'filename', and extracts
	it to the location spcified by 'path'
=====================================================================
*/
bool MpkFileReader::Extract(char* path, char* filename) {
	
	DWORD dwAttrib = GetFileAttributesA(path);

	//Check if path exists
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		errorString = "Error filename not found";
		return false;
	}

	//Check if the filename could be found in _fileData
	std::map<std::string, std::vector<char>>::iterator it = _fileData.find(filename);
	if (it != _fileData.end()) 
	{
		std::string filePath = path;
		filePath.append(filename);

		std::ofstream outStream;
		outStream.open(filePath, std::ios::binary);

		if (!outStream.is_open())
		{
			errorString = "Error opening file";
			return false;
		}

		outStream.write(&it->second[0],it->second.size());

		outStream.close();
	}
	else {
		errorString="Can't find data for file";
		return false;
	}

	return true;
}

std::map<std::string, std::vector<char>> MpkFileReader::GetArchiveData()
{
	return _fileData;
}


void MpkFileReader::Upload (int stage, char *data, int len) {

	switch (stage)
	{
		case 0:
		{
			_mpkName.append(data);
			break;
		}
		case 1: //Process filenames
		{
			//Accumulate data into buffer
			for (int i = 0; i < len; i++)
			{
				_filenamesBuffer.push_back(data[i]);
			}

			for (; _offset < _filenamesBuffer.size(); _offset += 0x11c)
			{
				////check for buffer "overflow"
				if(_offset + 0x11c > _filenamesBuffer.size())
					break;

				size_t nameLen = strlen(&_filenamesBuffer[_offset]);
				char *filename = new char[nameLen];
				//Accumulate data into buffer
				for (int i = 0; i <= nameLen; i++)
				{
					filename[i] = _filenamesBuffer[_offset + i];
				}

				_fileNames.push_back(filename);
			}
			break;
		}
		default: //stage > 1, file data
		{
			//find the iterator pointing to the relevant file data vector. If it does not exist, create it.
			std::map<std::string, std::vector<char>>::iterator it = _fileData.find(_fileNames[(stage - 2)]);
			if (it == _fileData.end())
			{
				std::vector<char> fileData;
				_fileData.insert(std::pair<std::string, std::vector<char>>(_fileNames[(stage - 2)], fileData));
				it = _fileData.find(_fileNames[(stage - 2)]);
			}

			for (int i = 0; i < len; i++)
				it->second.push_back(data[i]);
		}
	}

	_lastStage = stage;
}


