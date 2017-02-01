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
#include <sstream>      
#include <png.h>
#include <png++/png.hpp>
#include "MpkFileReader.h"
#include "PcxReader.h"

//TODO: This path needs to be another place.
static std::string zoneBasePath = "C:\\Projects\\DAOC\\Dark Age of Camelot\\zones\\";

enum Zones_e
{
	//All Realms
	Zone011, //Forest Sauvage    
	Zone012, //Snowdonia         
	Zone014, //Pennine Mountains 
	Zone015, //Hadrian's Wall    
	Zone111, //Uppland           
	Zone112, //Yggdra Forest     
	Zone113, //Jamtland Mountains
	Zone115, //Odin's Gate       
	Zone210, //Mount Collory     
	Zone211, //Cruachan Gorge    
	Zone212, //Breifine          
	Zone214, //Emain Macha    
	Zone249, //Darkness Falls

	//Albion
	Zone026, //Camelot				
	Zone000, //Camelot Hills		
	Zone001, //Salisbury Plains		
	Zone002, //Black Mountains South
	Zone003, //Black Mountains North
	Zone004, //Dartmoor				
	Zone006, //Cornwall				
	Zone007, //Llyn Barfog			
	Zone008, //Campacorentin Forest	
	Zone009, //Avalon Marsh			
	Zone010, //Lyonesse				
	Zone019, //Stonehenge Barrows	
	Zone021, //Tomb of Mithra		
	Zone022, //Keltoi Fogou			
	Zone023, //Catacombs of Cardova	
	Zone024, //Tepok's Mine			

	//Hibernia
	Zone209, //Tir na Nog			
	Zone200, //Lough Derg			
	Zone201, //Silvermine Mts.		
	Zone202, //Shannon Estuary		
	Zone203, //Cliffs of Moher		
	Zone204, //Lough Gur			
	Zone205, //Bog of Cullen		
	Zone206, //Valley of Bri Leith	
	Zone207, //Connacht				
	Zone208, //Cursed Forest		
	Zone216, //Sheeroe Hills		
	Zone220, //Coruscating Mines	
	Zone221, //Muire Tomb			
	Zone222, //Spraggon Den			
	Zone223, //Koalinth Caverns		
	Zone224, //Treibh Caillte		
	
	//Midgard
	Zone120, //Jordheim				
	Zone100, //Vale of Mularn		
	Zone101, //East Svealand		
	Zone102, //West Svealand		
	Zone103, //Gotar				
	Zone104, //Muspelheim			
	Zone105, //Myrkwood Forest		
	Zone106, //Skona Ravine			
	Zone107, //Vanern Swamp			
	Zone108, //Raumarik				
	Zone116, //Malmohus				
	Zone125, //Spindelhalla			
	Zone126, //Vendo Caverns		
	Zone127, //Varulvhamn			
	Zone128, //Cursed Tomb			
	Zone129, //Nisse's Lair
};

static std::map<Zones_e, std::string> zoneIdMap =
{
	//All Realms
	{Zone011, "011"},
	{Zone012, "012"},
	{Zone014, "014"},
	{Zone015, "015"},
	{Zone111, "111"},
	{Zone112, "112"},
	{Zone113, "113"},
	{Zone115, "115"},
	{Zone210, "210"},
	{Zone211, "211"},
	{Zone212, "212"},
	{Zone214, "214"},
	{Zone249, "249"},

	//Albion 
	{Zone026, "026"},
	{Zone000, "000"},
	{Zone001, "001"},
	{Zone002, "002"},
	{Zone003, "003"},
	{Zone004, "004"},
	{Zone006, "006"},
	{Zone007, "007"},
	{Zone008, "008"},
	{Zone009, "009"},
	{Zone010, "010"},
	{Zone019, "019"},
	{Zone021, "021"},
	{Zone022, "022"},
	{Zone023, "023"},
	{Zone024, "024"},

	//Hibernia
	{Zone209, "209"},
	{Zone200, "200"},
	{Zone201, "201"},
	{Zone202, "202"},
	{Zone203, "203"},
	{Zone204, "204"},
	{Zone205, "205"},
	{Zone206, "206"},
	{Zone207, "207"},
	{Zone208, "208"},
	{Zone216, "216"},
	{Zone220, "220"},
	{Zone221, "221"},
	{Zone222, "222"},
	{Zone223, "223"},
	{Zone224, "224"},

	//Midgard
	{Zone120, "120"},
	{Zone100, "100"},
	{Zone101, "101"},
	{Zone102, "102"},
	{Zone103, "103"},
	{Zone104, "104"},
	{Zone105, "105"},
	{Zone106, "106"},
	{Zone107, "107"},
	{Zone108, "108"},
	{Zone116, "116"},
	{Zone125, "125"},
	{Zone126, "126"},
	{Zone127, "127"},
	{Zone128, "128"},
	{Zone129, "129"},
};

class ZoneDataExtractor {
public:
	ZoneDataExtractor();
	~ZoneDataExtractor();
	void SetZone(Zones_e zoneDir);
	int GenerateHeightMap(std::string outputPath);		
	
private:
	std::map<std::string, std::map<std::string, std::string>> _sectorData;
	
	int WritePng(std::string file, std::vector<char>* data, short width, short height);
	int readSectorDat(std::vector<char>* data);
	int getSectorField(std::string label, std::string fieldName, std::string *result);

	Zones_e _zone;
	
};


#endif //ZONE_DATA_EXTRACTOR_H