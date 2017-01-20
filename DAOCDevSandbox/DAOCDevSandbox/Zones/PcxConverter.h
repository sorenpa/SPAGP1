#pragma once

#ifndef PCX_CONVERTER_H
#define PCX_CONVERTER_H


#include <iostream>
#include <fstream>

struct PcxHeader
{
	BYTE Identifier;        // PCX Id Number (Always 0x0A) // 10  
	BYTE Version;           // Version Number    // 5  
	BYTE Encoding;          // Encoding Format    // 1  
	BYTE BitsPerPixel;      // Bits per Pixel    // 8  
	WORD XStart;            // Left of image     // 0  
	WORD YStart;            // Top of Image     // 0  
	WORD XEnd;              // Right of Image    // 319  
	WORD YEnd;              // Bottom of image    // 199  
	WORD HorzRes;           // Horizontal Resolution   // 320  
	WORD VertRes;           // Vertical Resolution   // 200  
	BYTE Palette[48];       // 16-Color EGA Palette    
	BYTE Reserved1;         // Reserved (Always 0)  
	BYTE NumBitPlanes;      // Number of Bit Planes   // 1  
	WORD BytesPerLine;      // Bytes per Scan-line   // 320  
	WORD PaletteType;       // Palette Type     // 0  
	WORD HorzScreenSize;    // Horizontal Screen Size   // 0  
	WORD VertScreenSize;    // Vertical Screen Size   // 0  
	BYTE Reserved2[54];     // Reserved (Always 0)
};

class PcxConverter 
{
public:
	int readPCX(const char* file, char *data);
private:
	char* _imgData;
};

#endif PCX_CONVERTER_H