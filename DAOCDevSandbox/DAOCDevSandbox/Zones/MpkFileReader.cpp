#include "../stdafx.h"
#include "MpkFileReader.h"

MpkFileReader::MpkFileReader() 
{
	_lastStage = -1;
}

MpkFileReader::~MpkFileReader()
{
}

bool MpkFileReader::init(const char* file) {
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

bool MpkFileReader::extract(char* path, char* filename) {
	
	DWORD dwAttrib = GetFileAttributesA(path);

	//Check if path exists
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		errorString = "Error filename not found";
		return false;
	}

	//Check if the filePath could be found in _fileData
	std::map<std::string, std::pair<int, char*>>::iterator it = _fileData.find(filename);
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

		std::pair<int, char*> data = it->second;
		outStream.write(data.second,data.first);

		outStream.close();
	}
	else {
		errorString="Can't find data for file";
		return false;
	}

	return true;
}


void MpkFileReader::upload (int stage, char *data, int len) {

	//select the buffer
	std::vector<char> *buffer = &_fileDataBuffer;
	if (stage == 0)
		buffer = &_packetBuffer;
	if (stage == 1)
		buffer = &_filenamesBuffer;

	//Accumulate data into buffer
	for (int i = 0; i < len; i++)
	{
		buffer->push_back(data[i]);
	}

	switch (stage)
	{
		case 0:
		{
			std::string packetName(buffer->begin(), buffer->end());
			_mpkName.append(packetName);
			break;
		}
		case 1: //Process filenames
		{
			for (; _offset < buffer->size(); _offset += 0x11c)
			{
				//check for buffer "overflow"
				if(_offset + 0x11c > buffer->size())
					break;

				std::string filename(buffer->begin() + _offset, buffer->begin() + _offset + 0x11c);
				std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

				_fileNames.push_back(filename);
			}
			break;
		}
		default: //stage > 1, filePath data
		{
			if (stage != _lastStage) //new stage!
			{
				size_t bufLen = buffer->size()-len;
				char *fDataPtr = new char[bufLen];

				for (int i = 0; i < bufLen; i++)
					fDataPtr[i] = buffer->at(i);

				std::pair<int, char*> pair = std::pair<int, char*>(bufLen,fDataPtr);
				_fileData.insert(std::pair<std::string, std::pair<int, char*>>(_fileNames[(stage - 2)], pair));
				
				//the last portion of of the buffer, belongs to the next stage
				buffer->erase(buffer->begin(), buffer->end()-len);
			}
		}
	}



	_lastStage = stage;
}
