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
	std::map<std::string, std::vector<char>> mpkData = mpkReader.GetArchiveData();
	std::map<std::string, std::vector<char>>::iterator mpkIt = mpkData.find("SECTOR.DAT");
	if (mpkIt == mpkData.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find the SECTOR.DAT within .mpk archive" << _zone;
		return 1;
	}

	readSectorDat(&mpkIt->second);
	std::string scaleFactor;
	std::string offsetFactor;
	getSectorField("terrain", "scalefactor", &scaleFactor);
	getSectorField("terrain", "offsetfactor", &offsetFactor);
	
	//C: Read the relevant .pcx image files and add them (prop a helper func using PcxReader)
	PcxReader pcxReader = PcxReader();
	mpkIt = mpkData.find("terrain.pcx");
	if (mpkIt == mpkData.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find the SECTOR.DAT within .mpk archive" << _zone;
		return 1;
	}

	std::vector<char> terrainPcx = mpkIt->second;
	std::vector<char> terrainDecoded;
	pcxReader.readPCX(&terrainPcx, &terrainDecoded);
	WritePng("C:\\tmp\\terrain.png", &terrainDecoded,pcxReader._width, pcxReader._height);

	mpkIt = mpkData.find("offset.pcx");
	if (mpkIt == mpkData.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find the SECTOR.DAT within .mpk archive" << _zone;
		return 1;
	}

	int sFactor = atoi(scaleFactor.c_str());
	std::vector<unsigned int> terrainScaled;
	for (int i = 0; i < pcxReader._height;i++)
	{
		for (int j = 0; j < pcxReader._width;j++)
		{
			unsigned int px = (unsigned char)terrainDecoded.at(i*pcxReader._height + j) * sFactor;
			terrainScaled.push_back(px);
		}
	}

	std::vector<char> offsetPcx = mpkIt->second;
	std::vector<char> offsetDecoded;
	pcxReader.readPCX(&offsetPcx, &offsetDecoded);
	WritePng("C:\\tmp\\offset.png", &offsetDecoded, pcxReader._width, pcxReader._height);

	int oFactor = atoi(offsetFactor.c_str());
	std::vector<int> offsetScaled;
	for (int i = 0; i < pcxReader._height; i++)
	{
		for (int j = 0; j < pcxReader._width; j++)
		{
			int px = (unsigned char)offsetDecoded.at(i*pcxReader._height + j) * oFactor;
			offsetScaled.push_back(px);
		}
	}

	std::vector<char> heightMap;
	for (int i = 0; i < pcxReader._height; i++)
	{
		for (int j = 0; j < pcxReader._width; j++)
		{
			int px = (offsetScaled.at(i*pcxReader._height + j) + terrainScaled.at(i*pcxReader._height + j)) / (sFactor + oFactor);
			heightMap.push_back(px);
		}
	}

	//D: Write the Png to disk (using WritePng)
	WritePng("C:\\tmp\\hMap.png", &heightMap, pcxReader._width, pcxReader._height);

	return 0;
}

int ZoneDataExtractor::WritePng(std::string file, std::vector<char>* data, short width, short height)
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

int ZoneDataExtractor::readSectorDat(std::vector<char>* data)
{
	for (int i = 0; i < data->size(); i++)
	{
		while (data->at(i) != '[')
			i++;

		i++;
		std::string label;
		while (data->at(i) != ']')
		{
			label += data->at(i);
			i++;
		}

		i++;
		std::map<std::string, std::string> field;
		while (i < data->size() && data->at(i) != '[')
		{
			std::string name;
			std::string value;
			
			while (data->at(i) != '=')
			{
				if (data->at(i) != '\r' && data->at(i) != '\n')
				{
					name += data->at(i);
				}
				i++;
			}
			i++;
			while (data->at(i) != '\n')
			{
				if (data->at(i) != '\r' && data->at(i) != '\n')
				{
					value += data->at(i);
				}
				i++;
			}
			field.insert(std::pair<std::string, std::string>(name, value));
			
			while (data->at(i) == '\n' || data->at(i) == '\r')
			{
				i++;
				if (i+1 > data->size())
				{
					break;
				}
			}
		}
		_sectorData.insert(std::pair<std::string, std::map<std::string, std::string>>(label, field));
		i--;
	}
	return 0;
}

int ZoneDataExtractor::getSectorField(std::string label, std::string fieldName, std::string * result)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator labelIt = _sectorData.find(label);
	if (labelIt == _sectorData.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find label " << label << " within SECTOR.DAT for zone " << _zone;
		return 1;
	}

	std::map<std::string, std::string>::iterator fieldIt = labelIt->second.find(fieldName);
	if (fieldIt == labelIt->second.end())
	{
		std::cerr << "[ZoneDataExtractor]Error, could not find field " << fieldName << " within SECTOR.DAT for zone " << _zone;
		return 1;
	}

	*result = fieldIt->second;

	return 0;
}
