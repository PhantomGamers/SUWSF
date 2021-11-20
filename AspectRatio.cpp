#include "AspectRatio.h"
#include "dllmain.h"
#include "external/IniReader/IniReader.h"
#include "external/Blackbone/src/BlackBone/Patterns/PatternSearch.h"
#include "Logging.h"
#include "Memory.h"

using namespace blackbone;

void AspectRatio::Init()
{
	DBOUT("Initializing AspectRatio patch");
	AspectRatio::Patch(AspectRatio::GetConfig());
}

AspectRatio::Config AspectRatio::GetConfig()
{
	DBOUT("Getting AspectRatio config");
	CIniReader ini(APPNAME + ".ini");
	Config config;


	auto byteString = ini.ReadString("AspectRatio", "TargetBytes", "39 8E E3 3F");
	DBOUT("byteString is " << byteString);
	auto resString = ini.ReadString("AspectRatio", "DesiredResolution", "3440x1440");
	DBOUT("resString is " << resString);
	auto resStrings = split(resString, 'x');

	config.desiredWidth = std::stoi(resStrings.at(0));
	DBOUT("Width is " << config.desiredWidth);

	config.desiredHeight = std::stoi(resStrings.at(1));
	DBOUT("Height is " << config.desiredHeight);

	config.bytes = HexFromString(byteString);
	DBOUT("Proccessed bytes are " << config.bytes);

	return config;
}

void AspectRatio::Patch(Config config)
{
	DBOUT("Searching for bytes");
	PatternSearch ps{ config.bytes };

	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	MODULEINFO mInfo = GetModuleInfo((char*)szFileName);
	auto base = mInfo.lpBaseOfDll;
	auto size = mInfo.SizeOfImage;

	std::vector<ptr_t> results;
	auto matchesFound = ps.Search(base, size, results);
	DBOUT("Found " << matchesFound << " matches");

	for (int i = 0; i < results.size(); i++)
	{
		DBOUT("Writing to result " << i + 1);
		Memory::WriteFloat(results[i], (float)config.desiredWidth / config.desiredHeight);
	}
}