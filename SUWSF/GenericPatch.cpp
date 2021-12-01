#include "GenericPatch.h"
#include "dllmain.h"
#include "../external/IniReader/IniReader.h"
#include "Logging.h"
#include "boost/algorithm/string/replace.hpp"
#include "Memory.h"
#include "../external/Blackbone/src/BlackBone/Patterns/PatternSearch.h"
#include "UserSettings.h"
#include <string>

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
				boost::erase_all(params.second, "\"");
				config.val = params.second;
			}
			else if (params.first == "ValueType")
			{
				DBOUT("Found ValueType param with value " << params.second);
				boost::erase_all(params.second, "\"");
				config.valType = params.second;
				if (config.valType != "float" && config.valType != "byte")
				{
					DBOUT("ValueType unsupported. Supported types are: float, byte");
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
		if (config.valType == "float")
		{
			try
			{
				boost::replace_all(config.val, "width", std::to_string(UserSettings::config.width));
				boost::replace_all(config.val, "height", std::to_string(UserSettings::config.height));
				boost::replace_all(config.val, "aspectratio", std::to_string(UserSettings::config.aspectratio));
				float f = std::stof(config.val);
				config.val = reinterpret_cast<char*>(reinterpret_cast<BYTE*>(&f));
				config.val.pop_back(); // remove string terminator
			}
			catch (std::exception const& e)
			{
				DBOUT("Value is not a float, skipping patch...");
				goto CONTINUE;
			}
		}
		else if (config.valType == "byte")
		{
			config.val = HexFromString(config.val);
			if (config.val.length() == 0)
			{
				DBOUT("Value could not be read as bytes, skipping patch...");
				goto CONTINUE;
			}
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
			auto value = reinterpret_cast<BYTE*>(const_cast<char*>(config.val.c_str()));
			DBOUT("Writing " << value << " with length " << config.val.length() << " to result " << i + 1);
			Memory::Write(results[i] + config.offset, value, config.val.length());
		}
	}
}
