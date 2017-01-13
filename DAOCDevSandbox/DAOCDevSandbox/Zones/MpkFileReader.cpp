
#include "stdafx.h"
#include "MpkFileReader.h"

MpkFileReader::MpkFileReader() {
	last_stage=0;
}

bool MpkFileReader::init(char * file) {
	int inlen = 0;
	int stage = 0;
	char inbuf[16384],outbuf[16384];
	z_stream stream;
	
	FILE *fp;

	errno_t err;
	if ((err = fopen_s(&fp, file, "rb")) != 0) {
		errorString = "Error opening file";//,file; //TODO format string err msg
		return false;
	}

	// read and check header
	fseek(fp, 0, SEEK_SET);
	fread (inbuf,1,4,fp);
	inbuf[4]='\0';
	if ( strcmp("MPAK",inbuf) ) {
		errorString="Not a valid MPAK file"; //TODO format string  err msg
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
			errorString = "inflate returned"; //rc); //TODO err msg
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
	return(true);
}

bool MpkFileReader::extract(const wchar_t* path, char* filename) {
	
	DWORD dwAttrib = GetFileAttributes(path);

	//Check if path exists
	if (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		//TODO: print some error string
		return false;
	}

	HANDLE hFind;
	WIN32_FIND_DATA data;

	bool fileFound = false;
	hFind = FindFirstFile(path, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			//TODO: use strcmp to see if the file is in the dir
			printf("%ws\n", data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	if ( fileFound ) {
		FILE *fp;

		errno_t err;
		if ((err = fopen_s(&fp, filename, "rb")) != 0) {
			errorString = "Error opening file";//,file; //TODO format string err msg
			return false;
		}

		//Todo write something to the file

		fclose(fp);

	} else {
		errorString="Can't find data for file";
		return false;
	}

	return true;
}



void MpkFileReader::upload (int stage,char *data,int len) {
	// actions
	if ( stage != last_stage ) {
		switch ( last_stage )
		{
			case 0:
				//packet.append(QString(packetname_block.trimmed().toLower()));
				//TODO append packetname_block.trimmed().toLower() to packet
				break;
			case 1:
				for (int i=0 ; i < strlen(filenames_block) ; i += 0x11c )
					//fileNames.append(QString(filenames_block.mid(i,0x11c).trimmed().toLower()));
					//TODO append filenames_block.mid(i,0x11c).trimmed().toLower()
				break;
		}
	}

	if ( stage == 0 )
		//packetname_block.append(data,len);	
		//TODO
	if ( stage == 1 )
		//filenames_block.append(data,len);
		//TODO
	if ( stage > 1) //&& fileNames[stage-2] != nullptr )//TODO
		//file_data[ fileNames.at((stage-2)) ].append(data,len);
		//TODO

	last_stage=stage;		
}
