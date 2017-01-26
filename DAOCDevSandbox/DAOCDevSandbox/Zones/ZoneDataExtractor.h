/*
===============================================================================
ZoneDataExtractor.h

Class able to extract data for a DAOC Zone for use in the UE4 game engine.

===============================================================================
*/
#pragma once

#ifndef ZONE_DATA_EXTRACTOR_H
#define ZONE_DATA_EXTRACTOR_H

#include <iostream>
#include <fstream>
#include <png.h>
#include <png++/png.hpp>
#include "MpkFileReader.h"
#include "PcxReader.h"

//TODO this should be moved to a more convinient place
#define ZONE_DIR "C:/../../.."

enum Zones_e
{
	Zone105, //Myrkwood Forrest
};

class ZoneDataExtractor {
public:
	ZoneDataExtractor();
	~ZoneDataExtractor();
	void SetZone(Zones_e zoneDir);
	int GenerateHeightMap(std::string outputPath);
private:
	Zones_e _zone;
	int WritePng(std::string file, std::vector<unsigned char>* data, short width, short height);
};


#endif //ZONE_DATA_EXTRACTOR_H