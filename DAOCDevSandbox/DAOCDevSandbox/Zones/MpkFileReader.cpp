#include "stdafx.h"
#include "MpkFileReader.h"

MpkFileReader::MpkFileReader() 
{
	_lastStage = -1;
}

MpkFileReader::~MpkFileReader()
{
}

bool MpkFileReader::init(const char* file) {
	int inlen = 0;
	int stage = 0;
	char inbuf[16384],outbuf[16384];
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
			upload(stage, outbuf, (char*)stream.next_out - outbuf);

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

bool MpkFileReader::extract(const char* path, const char* filename) {
	
	DWORD dwAttrib = GetFileAttributesA(path);

	//Check if path exists
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		errorString = "Error filename not found";
		return false;
	}

	//Check if the file could be found in _fileData
	std::map<std::string, std::string>::iterator it = _fileData.find(filename);
	if (it != _fileData.end()) 
	{
		FILE *fp;

		//TODO: add path to file name
		std::string file = path;
		file.append(filename);

		//TODO:Create file if it does not exist

		errno_t err;
		if ((err = fopen_s(&fp, filename, "wb")) != 0) {
			errorString = "Error opening file";
			return false;
		}

		const char* buffer = it->second.c_str();

		fwrite(buffer, sizeof(char), sizeof(buffer), fp);

		fclose(fp);
	}
	else {
		errorString="Can't find data for file";
		return false;
	}

	return true;
}



void MpkFileReader::upload (int stage, char *data, int len) {
	// actions
	switch ( stage )
	{
		int trimIndex;
		case 0:
			_mpkName.append(data,len);
			break;
		case 1: //Process filenames
			for (int offset = 0; offset < len; offset += 0x11c)
			{
				char fileName[1024];
				char* p;
				int i;

				for (i = 0; i < 0x11c; i++)
				{

					if (data[offset + i] == '\0')
						break;
					
					fileName[i] = data[offset + i];
				}
				fileName[i] = '\0';
				
				//To lower
				p = fileName;
				while (*p)
				{
					*p = tolower(*p), p++;
				}

				_fileNames.push_back(fileName);
			}
			break;
		default: //stage > 1, file data
			if (stage != _lastStage)
			{
				_fileData.insert(std::pair<std::string, std::string>(_fileNames[(stage - 2)], data));
				break;
			}

			std::map<std::string, std::string>::iterator it = _fileData.find(_fileNames[(stage - 2)]);
			if (it != _fileData.end())
			{
				it->second.append(data, len);
			}
	}

	_lastStage = stage;
}
