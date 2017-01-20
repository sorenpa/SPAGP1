#include "../stdafx.h"
#include "PcxConverter.h"

int PcxConverter::readPCX(const char* file, char* data)
{
	PcxHeader header;
	
	std::ifstream pcxFile;
	pcxFile.open(file, std::ios::binary);

	if (!pcxFile.is_open())
	{
		std::cout << "Error reading " << file << " - could not open";
		return 1;
	}

	//Read the header:
	header.Identifier     = pcxFile.get();          
	header.Version        = pcxFile.get();           
	header.Encoding       = pcxFile.get();          
	header.BitsPerPixel   = pcxFile.get();      
	header.XStart         = pcxFile.get();            
	header.YStart         = pcxFile.get();            
	header.XEnd			  = pcxFile.get();              
	header.YEnd			  = pcxFile.get();              
	header.HorzRes        = pcxFile.get();           
	header.VertRes        = pcxFile.get();           
	header.Palette[48]    = pcxFile.get();       
	header.Reserved1      = pcxFile.get();         
	header.NumBitPlanes   = pcxFile.get();      
	header.BytesPerLine   = pcxFile.get();      
	header.PaletteType    = pcxFile.get();       
	header.HorzScreenSize = pcxFile.get();    
	header.VertScreenSize = pcxFile.get();    
	header.Reserved2[54]  = pcxFile.get();     


	pcxFile.close();
	
}