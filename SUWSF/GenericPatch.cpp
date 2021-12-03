#include "GenericPatch.h"
#include "dllmain.h"
#include "../external/IniReader/IniReader.h"
#include "Logging.h"
#include "boost/algorithm/string/replace.hpp"
#include "Memory.h"
#include "UserSettings.h"
#include <string>
#include "../external/tinyexpr/tinyexpr.h"
#include "../external/Hooking.Patterns/Hooking.Patterns.h"

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
	for (auto& entry : ini.data)
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
				boost::replace_all(config.pattern, "??", "?");
				boost::erase_all(config.pattern, "\"");
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
			else if (params.first == "Match")
			{
				boost::erase_all(params.second, "\"");
				config.matches = params.second;
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
				int error = -1;
				float f = te_interp(config.val.c_str(), &error);

				if (error != 0)
				{
					DBOUT("Could not interpret value expression, skipping patch...");
					goto CONTINUE;
				}

				config.val = reinterpret_cast<char*>(reinterpret_cast<BYTE*>(&f));
				while (config.val.size() > 4)
				{
					config.val.pop_back();
				}
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
	for (auto& config : configs)
	{
		DBOUT("Searching for bytes " << config.pattern);
		auto pattern = hook::pattern((uintptr_t)mInfo.lpBaseOfDll, (uintptr_t)mInfo.lpBaseOfDll + mInfo.SizeOfImage, config.pattern);
		DBOUT("Found " << pattern.size() << " matches");

		if (pattern.size() == 0)
		{
			continue;
		}

		auto value = reinterpret_cast<BYTE*>(const_cast<char*>(config.val.c_str()));
		if (config.matches == "all")
		{
			for (int i = 0; i < pattern.size(); i++)
			{
				DBOUT("Writing " << hexStr(value, config.val.length()) << " with length " << config.val.length() << " to result " << i + 1);
				Memory::Write(pattern.get(i).get<BYTE>(config.offset), value, config.val.length());
			}
		}
		else
		{
			int match = -1;
			if (config.matches == "last" || config.matches == "end")
			{
				match = pattern.size();
			}
			else
			{
				try
				{
					match = std::stoi(config.matches);
				}
				catch (std::exception const& e)
				{
					DBOUT("Could not parse match number as integer, skipping...");
					continue;
				}
			}

			if (match < 1 || match > pattern.size())
			{
				DBOUT("Match setting invalid, must resolve to a value between 1 and " << pattern.size());
				continue;
			}

			DBOUT("Writing " << hexStr(value, config.val.length()) << " with length " << config.val.length() << " to match " << match);
			Memory::Write(pattern.get(match - 1).get<BYTE>(config.offset), value, config.val.length());
		}
	}
}