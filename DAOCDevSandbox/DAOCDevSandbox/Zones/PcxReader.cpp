#include "../stdafx.h"
#include "PcxReader.h"

int PcxReader::readPCX(const char* file, std::vector<unsigned char>* image)
{
	std::ifstream pcxFile;
	pcxFile.open(file, std::ios::binary);

	if (!pcxFile.is_open())
	{
		std::cout << "[PcxConverter]Error reading " << file << " - could not open";
		return 1;
	}

	//Read the header:
	pcxFile.read(reinterpret_cast<char*>(&_header.Identifier),     sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.Version),        sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.Encoding),       sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.BitsPerPixel),   sizeof(char));
	//DWORD														  
	pcxFile.read(reinterpret_cast<char*>(&_header.XStart),         sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.YStart),         sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.XEnd),           sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.YEnd),           sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.HorzRes),        sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.VertRes),        sizeof(short));
	//BYTE
	pcxFile.read(reinterpret_cast<char*>(_header.Palette), 48);
	pcxFile.read(reinterpret_cast<char*>(&_header.Reserved1),      sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.NumBitPlanes),   sizeof(char));
	//DWORD
	pcxFile.read(reinterpret_cast<char*>(&_header.BytesPerLine),   sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.PaletteType),    sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.HorzScreenSize), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.VertScreenSize), sizeof(short));

	pcxFile.ignore(54);

	//Debug
	char endl = '\n';
	std::cout 
		<< "manufacturer: "			<< (short)_header.Identifier      << endl
		<< "version: "				<< (short)_header.Version         << endl
		<< "encoding: "				<< (short)_header.Encoding        << endl
		<< "bitsPerPixel: "			<< (short)_header.BitsPerPixel    << endl
		<< "XStart: "				<< _header.XStart          << endl
		<< "YStart: "				<< _header.YStart          << endl
		<< "XEnd: "					<< _header.XEnd            << endl
		<< "YEnd: "					<< _header.YEnd            << endl
		<< "verticalRes: "			<< _header.VertRes << endl
		<< "horizontalRes: "		<< _header.HorzRes << endl
		<< "reserved1: "			<< _header.Reserved1 << endl
		<< "bitPlanes: "			<< _header.NumBitPlanes << endl
		<< "bytesPerLine: "		    << _header.BytesPerLine    << endl
		<< "paletteType: "          << _header.PaletteType     << endl
		<< "horizontalScreenSize: " << _header.HorzScreenSize  << endl
		<< "verticalScreenSize: "   << _header.VertScreenSize  << endl;

	_width = _header.XEnd - _header.XStart + 1;
	_height = _header.YEnd - _header.YStart + 1;

	std::vector<unsigned char> imageData;
	int scanLineLength = _header.BytesPerLine * _header.NumBitPlanes;
	int bytesPerLine;
	unsigned char count; //RLE runcount
	char byte;
	for (int lineCount = 0; lineCount < _height; ++lineCount)
	{
		bytesPerLine = scanLineLength;
		switch (_header.Encoding) {
		case 0: //uncompressed image
			do {
				pcxFile.read(&byte, sizeof(char));
				imageData.push_back(byte);
				--bytesPerLine;
			} while (bytesPerLine != 0);
			break;
		case 1: //RLE decoding of one line
			do {
				pcxFile.read(&byte, sizeof(char));
				if ((byte & 0xC0) == 0xC0) {
					count = byte & 0x3F;
					pcxFile.read(&byte, sizeof(char));
				}
				else {
					count = 1;
				}

				//Write the pixel run to the buffer
				for (int j = 0; j < count; j++) {
					imageData.push_back(byte);
					--bytesPerLine;
				}

				if (bytesPerLine < 0)
				{
					pcxFile.close();
					std::cout << "[PcxConverter]Error reading " << file << " - bytes per line was negative";
					return 1;
				}
			} while (bytesPerLine != 0);
			break;
		}
	}

	switch ((short)_header.BitsPerPixel*(short)_header.NumBitPlanes) {
	case 1:
	case 4:
	case 24:
	case 32:
		pcxFile.close();
		std::cout << "[PcxConverter]Error reading " << file << " - unsupported .pcx file";
		return 1; //Unsopprted
	default:
		break;
	}

	if ((short)_header.BitsPerPixel == 8 && (short)_header.NumBitPlanes == 1) {
		switch (_header.PaletteType) {
		case 1: // Color/B&W
		{
			pcxFile.close();
			std::cout << "[PcxConverter]Error reading " << file << " - unsupported .pcx file";
			return 1; //Unsopprted
			break;
		} 
		case 2: // grayscale
			break;
		default: 
			pcxFile.close();
			std::cout << "[PcxConverter]Error reading " << file << " - unsupported palette type";
			return 1; //Unsopprted
			break;
		}
	}

	*image = imageData;
	return 0;
}

int PcxReader::readPCX(std::vector<char> *data, std::vector<char>* image)
{
	std::string str(data->begin(), data->end());
	std::stringstream pcxFile;
	
	pcxFile.write(&str[0], data->size());
	

	//Read the header:
	pcxFile.read(reinterpret_cast<char*>(&_header.Identifier), sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.Version), sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.Encoding), sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.BitsPerPixel), sizeof(char));
	//DWORD														  
	pcxFile.read(reinterpret_cast<char*>(&_header.XStart), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.YStart), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.XEnd), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.YEnd), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.HorzRes), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.VertRes), sizeof(short));
	//BYTE
	pcxFile.read(reinterpret_cast<char*>(_header.Palette), 48);
	pcxFile.read(reinterpret_cast<char*>(&_header.Reserved1), sizeof(char));
	pcxFile.read(reinterpret_cast<char*>(&_header.NumBitPlanes), sizeof(char));
	//DWORD
	pcxFile.read(reinterpret_cast<char*>(&_header.BytesPerLine), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.PaletteType), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.HorzScreenSize), sizeof(short));
	pcxFile.read(reinterpret_cast<char*>(&_header.VertScreenSize), sizeof(short));

	pcxFile.ignore(54);

	//Debug
	char endl = '\n';
	std::cout
		<< "manufacturer: " << (short)_header.Identifier << endl
		<< "version: " << (short)_header.Version << endl
		<< "encoding: " << (short)_header.Encoding << endl
		<< "bitsPerPixel: " << (short)_header.BitsPerPixel << endl
		<< "XStart: " << _header.XStart << endl
		<< "YStart: " << _header.YStart << endl
		<< "XEnd: " << _header.XEnd << endl
		<< "YEnd: " << _header.YEnd << endl
		<< "verticalRes: " << _header.VertRes << endl
		<< "horizontalRes: " << _header.HorzRes << endl
		<< "reserved1: " << _header.Reserved1 << endl
		<< "bitPlanes: " << (short)_header.NumBitPlanes << endl
		<< "bytesPerLine: " << _header.BytesPerLine << endl
		<< "paletteType: " << _header.PaletteType << endl
		<< "horizontalScreenSize: " << _header.HorzScreenSize << endl
		<< "verticalScreenSize: " << _header.VertScreenSize << endl;

	_width = _header.XEnd - _header.XStart + 1;
	_height = _header.YEnd - _header.YStart + 1;

	std::vector<char> imageData;
	int scanLineLength = _header.BytesPerLine * _header.NumBitPlanes;
	int bytesPerLine;
	unsigned char count; //RLE runcount
	char byte;
	for (int lineCount = 0; lineCount < _height; ++lineCount)
	{
		bytesPerLine = scanLineLength;
		switch (_header.Encoding) {
		case 0: //uncompressed image
			do {
				pcxFile.read(&byte, sizeof(char));
				imageData.push_back(byte);
				--bytesPerLine;
			} while (bytesPerLine != 0);
			break;
		case 1: //RLE decoding of one line
			do {
				pcxFile.read(&byte, sizeof(char));
				if ((byte & 0xC0) == 0xC0) {
					count = byte & 0x3F;
					pcxFile.read(&byte, sizeof(char));
				}
				else {
					count = 1;
				}

				//Write the pixel run to the buffer
				for (int j = 0; j < count; j++) {
					imageData.push_back(byte);
					--bytesPerLine;
				}

				if (bytesPerLine < 0)
				{
					std::cout << "[PcxConverter]Error reading " << data << " - bytes per line was negative";
					return 1;
				}
			} while (bytesPerLine != 0);
			break;
		}
	}

	switch ((short)_header.BitsPerPixel*(short)_header.NumBitPlanes) {
	case 1:
	case 4:
	case 24:
	case 32:

		std::cout << "[PcxConverter]Error reading " << data << " - unsupported .pcx file";
		return 1; //Unsopprted
	default:
		break;
	}

	if ((short)_header.BitsPerPixel == 8 && (short)_header.NumBitPlanes == 1) {
		switch (_header.PaletteType) {
		case 1: // Color/B&W
		{
			std::cout << "[PcxConverter]Error reading " << data << " - unsupported .pcx file";
			return 1; //Unsopprted
			break;
		}
		case 2: // grayscale
			break;
		default:
			std::cout << "[PcxConverter]Error reading " << data << " - unsupported palette type";
			return 1; //Unsopprted
			break;
		}
	}

	*image = imageData;
	return 0;
}
