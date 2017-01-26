#include "../stdafx.h"
#include "ZoneDataExtractor.h"

ZoneDataExtractor::ZoneDataExtractor() 
{
}

ZoneDataExtractor::~ZoneDataExtractor() 
{
}

void ZoneDataExtractor::SetZone(Zones_e zone)
{
	_zone = zone;
}

int ZoneDataExtractor::GenerateHeightMap(std::string outputPath) 
{
	std::map<std::string, std::vector<char>> archiveData;

	//A: Extract the relevant data_something.mpk file (using MpkFileReader)
	std::map<Zones_e, std::string>::iterator it = zoneIdMap.find(_zone);
	if (it == zoneIdMap.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find the zone " << _zone;
		return 1;
	}

	std::string zoneDir = zoneBasePath + "Zone" + it->second + "\\";
	std::string datMpkFileName = "dat" + it->second + ".mpk";
	const char * c = zoneDir.append(datMpkFileName).c_str();
	MpkFileReader mpkReader = MpkFileReader();
	mpkReader.Decompress(c);

	//B: Read the data file (prop a helper func)
	//C: Read the relevant .pcx image files and add them (prop a helper func using PcxReader)
	//D: Write the Png to disk (using WritePng)
	
	return 0;
}

int ZoneDataExtractor::WritePng(std::string file, std::vector<unsigned char>* data, short width, short height)
{
	png::image< png::gray_pixel > image(width, height);
	for (size_t y = 0; y < height; ++y)
	{
		for (size_t x = 0; x < width; ++x)
		{
			png::byte b = (png::byte)data->at(x*width + y);
			image[x][y] = png::gray_pixel(b);
		}
	}

	image.write(file);

	return 0;
}