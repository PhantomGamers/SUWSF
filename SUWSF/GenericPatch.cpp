#include "GenericPatch.h"
#include "dllmain.h"
#include "../external/IniReader/IniReader.h"
#include "Logging.h"
#include "boost/algorithm/string/replace.hpp"
#include "Memory.h"
#include "../external/Blackbone/src/BlackBone/Patterns/PatternSearch.h"
#include "UserSettings.h"

using namespace blackbone;

void GenericPatch::Init()
{
	DBOUT("Initializing GenericPatch patches");
	PatchAll(GetConfigs());
}

std::vector<GenericPatch::Config> GenericPatch::GetConfigs()
{
	CIniReader ini(ININAME);

	DBOUT("Searching for patches...");

	std::vector<Config> configs;
	for (auto &entry : ini.data)
	{
		if (entry.first.find("Patch") == std::string::npos)
			continue;

		DBOUT("Found a patch!");

		Config config;

		for (auto& params : entry.second)
		{
			if (params.first == "Pattern")
			{
				DBOUT("Found pattern param with value " << params.second);
				config.pattern = params.second;
				config.wildcardEnabled = config.pattern.find("??") != std::string::npos;
				boost::replace_all(config.pattern, "??", config.wildcardChar);
				boost::erase_all(config.pattern, "\"");
				config.pattern = HexFromString(config.pattern);
			}
			else if (params.first == "Offset")
			{
				DBOUT("Found offset param with value " << params.second);
				try
				{
					config.offset = std::stoi(params.second);
				}
				catch (std::exception const& e)
				{
					DBOUT("Offset is not an integer, skipping patch...");
					goto CONTINUE;
				}
				if (config.offset < 0 || (config.pattern.length() - config.offset) < 4)
				{
					DBOUT("Offset invalid, should be between 0 and " << (config.pattern.length() - 4));
				}
			}
			else if (params.first == "Value")
			{
				DBOUT("Found value param with value " << params.second);
				try
				{
					boost::replace_all(params.second, "width", std::to_string(UserSettings::config.width));
					boost::replace_all(params.second, "height", std::to_string(UserSettings::config.height));
					boost::replace_all(params.second, "aspectratio", std::to_string(UserSettings::config.aspectratio));
					boost::erase_all(params.second, "\"");
					config.val = std::stof(params.second);
				}
				catch (std::exception const& e)
				{
					DBOUT("Value is not a float, skipping patch...");
					DBOUT("Failed with " << e.what());
					goto CONTINUE;
				}
			}
			else if (params.first == "WildcardChar")
			{
				config.wildcardChar = HexFromString(params.second);
				if (config.wildcardChar.length() != 1)
				{
					DBOUT("Wildcard is not valid, should be one byte, skipping patch...");
					goto CONTINUE;
				}
			}
			else if (params.first == "Enabled")
			{
				try
				{
					if (!boost::lexical_cast<bool>(params.second))
					{
						goto CONTINUE;
					}
				}
				catch (boost::bad_lexical_cast const& e)
				{
					DBOUT("Could not parse Enabled param, skipping patch...");
					goto CONTINUE;
				}
			}
		}
		if (config.pattern.length() == 0)
		{
			DBOUT("No pattern found, skipping patch...");
			goto CONTINUE;
		}
		configs.push_back(config);
	CONTINUE:
		continue;
	}
	DBOUT(configs.size() << " patches found");
	return configs;
}

void GenericPatch::PatchAll(std::vector<Config> configs)
{
	for (auto &config : configs)
	{
		DBOUT("Searching for bytes " << config.pattern);
		PatternSearch ps{ config.pattern };

		std::vector<ptr_t> results;
		size_t matchesFound = 0;
		if (config.wildcardEnabled)
		{
			matchesFound = ps.Search(reinterpret_cast<const uint8_t>(&config.wildcardChar[0]), mInfo.lpBaseOfDll, mInfo.SizeOfImage, results);
		}
		else
		{
			matchesFound = ps.Search(mInfo.lpBaseOfDll, mInfo.SizeOfImage, results);
		}

		DBOUT("Found " << matchesFound << " matches");

		for (int i = 0; i < results.size(); i++)
		{
			DBOUT("Writing to result " << i + 1);
			Memory::WriteFloat(results[i] + config.offset, config.val);
		}
	}
}
