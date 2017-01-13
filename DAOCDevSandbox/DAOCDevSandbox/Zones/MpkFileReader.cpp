#include "stdafx.h"
#include "MpkFileReader.h"

MpkFileReader::MpkFileReader() {
	last_stage=0;
}

MpkFileReader::~MpkFileReader()
{
}

bool MpkFileReader::init(const wchar_t * file) {
	int inlen = 0;
	int stage = 0;
	char inbuf[16384],outbuf[16384];
	z_stream stream;
	
	FILE *fp;

	errno_t err;
	if ((err = _wfopen_s(&fp, file, L"rb")) != 0) {
		errorString = "Error opening file";
		return false;
	}

	// read and check header
	fseek(fp, 0, SEEK_SET);
	fread (inbuf,1,4,fp);
	inbuf[4]='\0';
	if ( strcmp("MPAK",inbuf) ) {
		errorString="Not a valid MPAK file";
		return(false);	
	}
	// zstream start
	fseek(fp, 21, SEEK_SET);
	
	memset(&stream,0,sizeof(z_stream));

	inflateInit(&stream); 
	while (!feof(fp) || inlen > 0) {
		int rc=0;
		inlen += fread(inbuf + inlen, 1, sizeof(inbuf) - inlen, fp);
		stream.next_in = (unsigned char *)inbuf;
		stream.avail_in = inlen;
		stream.next_out = (unsigned char *)outbuf;
		stream.avail_out = sizeof(outbuf);
		rc = inflate(&stream, 0);
		if (rc != Z_STREAM_END && rc != Z_OK) {
			errorString = "inflate returned"; 
			return(false);
		}
        if ((char*)stream.next_out > outbuf)
			upload(stage, outbuf, (char*)stream.next_out - outbuf);

		if ((char*)stream.next_in > inbuf) {
			memmove(inbuf, stream.next_in, stream.avail_in);
			inlen = stream.avail_in;
		}
		if (rc == Z_STREAM_END) {
			++stage;
			inflateEnd(&stream);
			inflateInit(&stream);
		}
    }
	inflateEnd(&stream);
	fclose(fp);
	return true;
}

bool MpkFileReader::extract(const wchar_t* path, const wchar_t* filename) {
	
	DWORD dwAttrib = GetFileAttributes(path);

	//Check if path exists
	if (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		errorString = "Error filename not found";
		return false;
	}

	HANDLE hFind;
	WIN32_FIND_DATA data;

	//TODO chack if file_data containts the file name
	bool fileFound = false;
	if ( fileFound ) {
		FILE *fp;

		errno_t err;
		if ((err = _wfopen_s(&fp, filename, L"rb")) != 0) {
			errorString = "Error opening file";
			return false;
		}

		//TODO write something to the file
		//file.write(file_data[filename]);

		fclose(fp);

	} else {
		errorString="Can't find data for file";
		return false;
	}

	return true;
}



void MpkFileReader::upload (int stage, char *data, int len) {
	// actions
	if ( stage != last_stage ) {
		switch ( last_stage )
		{
			int trimIndex;
			case 0:
				//packet.append(QString(packetname_block.trimmed().toLower()));
				//TODO append packetname_block.trimmed().toLower() to packet
				
				//TODO trim + lowercase

				//append packetname_block to packet;
				packet.append(packetname_block);
				break;
			case 1:
				for (int i = 0; i < filenames_block.size(); i += 0x11c)
				{
				//	/*
				//	QByteArray::mid(int pos, int len = -1) const

				//	Returns a byte array containing len bytes from this byte array, starting at position pos.

				//	If len is -1 (the default), or pos + len >= size(), returns a byte array containing all bytes starting at position pos until the end of the byte array.
				//	
				//	//fileNames.append(QString(filenames_block.mid(i,0x11c).trimmed().toLower()));
				//	//TODO append filenames_block.mid(i,0x11c).trimmed().toLower()
				//	*/

					char block[0x11c];
					int j = 0;
					for (j = 0; j < filenames_block.size(); j++)
					{
							char c = 
							block[j] = filenames_block[i + j];
					}
					block[j] = '\0';

					//TODO: trim + lowercase

					fileNames.push_back(block);
				}
				break;
		}
	}

	if (stage == 0)
	{
		packetname_block.append(data);
	}
	
	if (stage == 1)
	{
		filenames_block.append(data);
	}

	if (stage > 1)//TODO
	{
		file_data.insert(std::pair<std::string,std::string>(fileNames[(stage - 2)], data));
	}

	last_stage=stage;		
}
