/*
===============================================================================
PcxReader

Class able to read .pcx image files into memory. Only a subset of the .pcx file
format has been implemented, since this class is only intended to read zone
data from the original DAOC client.
===============================================================================
*/
#pragma once

#ifndef PCX_READER_H
#define PCX_READER_H

#include <iostream>
#include <fstream>
#include <sstream>

struct PcxHeader
{
	unsigned char Identifier;        // PCX Id Number (Always 0x0A) // 10  
	unsigned char Version;           // Version Number    // 5  
	unsigned char Encoding;          // Encoding Format    // 1  
	unsigned char BitsPerPixel;      // Bits per Pixel    // 8  
	unsigned short XStart;            // Left of image     // 0  
	unsigned short YStart;            // Top of Image     // 0  
	unsigned short XEnd;              // Right of Image    // 319  
	unsigned short YEnd;              // Bottom of image    // 199  
	unsigned short HorzRes;           // Horizontal Resolution   // 320  
	unsigned short VertRes;           // Vertical Resolution   // 200  
	unsigned char Palette[48];       // 16-Color EGA Palette    
	unsigned char Reserved1;         // Reserved (Always 0)  
	unsigned char NumBitPlanes;      // Number of Bit Planes   // 1  
	unsigned short BytesPerLine;      // Bytes per Scan-line   // 320  
	unsigned short PaletteType;       // Palette Type     // 0  
	unsigned short HorzScreenSize;    // Horizontal Screen Size   // 0  
	unsigned short VertScreenSize;    // Vertical Screen Size   // 0  
	unsigned char Reserved2[54];     // Reserved (Always 0)
};

class PcxReader
{
public:
	int readPCX(const char* file, std::vector<unsigned char> *data);
	int readPCX(std::vector<char> *data, std::vector<char> *image);
	short _width;
	short _height;
private:
	PcxHeader _header;
	char* _imgData;
};

#endif //PCX_READER_H