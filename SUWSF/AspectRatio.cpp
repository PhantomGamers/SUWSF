#include "AspectRatio.h"
#include "dllmain.h"
#include "../external/IniReader/IniReader.h"
#include "../external/Blackbone/src/BlackBone/Patterns/PatternSearch.h"
#include "Logging.h"
#include "Memory.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

using namespace blackbone;

void AspectRatio::Init()
{
	DBOUT("Initializing AspectRatio patch");
	Patch(GetConfig());
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
	std::vector<std::string> resStrings;
	split(resStrings, resString, boost::is_any_of("x"));

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
	PatternSearch ps{config.bytes};

	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(nullptr, szFileName, MAX_PATH);
	MODULEINFO mInfo = GetModuleInfo(szFileName);
	auto base = mInfo.lpBaseOfDll;
	auto size = mInfo.SizeOfImage;

	std::vector<ptr_t> results;
	auto matchesFound = ps.Search(base, size, results);
	DBOUT("Found " << matchesFound << " matches");

	for (int i = 0; i < results.size(); i++)
	{
		DBOUT("Writing to result " << i + 1);
		Memory::WriteFloat(results[i], static_cast<float>(config.desiredWidth) / config.desiredHeight);
	}
}
