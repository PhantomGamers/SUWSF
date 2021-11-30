#include "UserSettings.h"
#include "dllmain.h"
#include "../external/IniReader/IniReader.h"
#include "Logging.h"
#include <boost/algorithm/string/split.hpp>
#include "boost/algorithm/string/classification.hpp"

UserSettings::Config UserSettings::config;

void UserSettings::Init()
{
	SetConfig();
}

void UserSettings::SetConfig()
{
	DBOUT("Getting UserSettings config");
	CIniReader ini(ININAME);

	auto resString = ini.ReadString("UserSettings", "Resolution", "auto");

	if (resString == "auto")
	{
		DBOUT("Automatically detecting resolution...");
		config.width = GetSystemMetrics(SM_CXSCREEN);
		config.height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		DBOUT("Parsing resolution " << resString);
		std::vector<std::string> resStrings;
		split(resStrings, resString, boost::is_any_of("x"));
		config.width = std::stoi(resStrings.at(0));
		config.height = std::stoi(resStrings.at(1));
	}
	config.aspectratio = (float)config.width / config.height;
	DBOUT("Detected width is " << config.width);
	DBOUT("Detected height is " << config.height);
	DBOUT("Detected aspect ratio is " << config.aspectratio);
	config.isSet = true;
}