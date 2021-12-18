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

	std::vector<Config> configs;
	if (!UserSettings::config.enabled)
	{
		DBOUT("Patches disabled by UserSettings");
		return configs;
	}

	DBOUT("Searching for patches...");

	for (auto& entry : ini.data)
	{
		if (entry.first.find("Patch") == std::string::npos)
			continue;

		DBOUT("Found patch " << entry.first);

		Config config;

		for (auto& params : entry.second)
		{
			boost::erase_all(params.second, "\"");
			if (params.first == "Pattern")
			{
				DBOUT("Pattern=" << params.second);
				config.pattern = params.second;
				boost::replace_all(config.pattern, "??", "?");
			}
			else if (params.first == "Offset")
			{
				DBOUT("Offset=" << params.second);
				try
				{
					config.offset = std::stoi(params.second);
				}
				catch (std::exception const& e)
				{
					DBOUT("Offset is not an integer, skipping patch...");
					goto CONTINUE;
				}
				if (config.offset < 0)
				{
					DBOUT("Offset invalid, must be greater than or equal to 0. Skipping patch...");
					goto CONTINUE;
				}
			}
			else if (params.first == "Value")
			{
				DBOUT("Value=" << params.second);
				config.val = params.second;
			}
			else if (params.first == "ValueType")
			{
				DBOUT("ValueType=" << params.second);
				config.valType = params.second;
				if (config.valType != "float" && config.valType != "byte" && config.valType != "int")
				{
					DBOUT("ValueType unsupported. Supported types are: float, byte...Skipping patch...");
					goto CONTINUE;
				}
			}
			else if (params.first == "Match")
			{
				DBOUT("Match=" << params.second);
				config.matches = params.second;
			}
			else if (params.first == "Enabled")
			{
				DBOUT("Enabled=" << params.second);
				if (params.second != "true")
				{
					DBOUT("Patch not enabled, skipping patch...");
					goto CONTINUE;
				}
			}
			else if (params.first == "Module")
			{
				DBOUT("Module=" << params.second);
				config.module = params.second;
			}
		}
		if (config.pattern.length() == 0)
		{
			DBOUT("No pattern found, skipping patch...");
			goto CONTINUE;
		}
		if (config.valType == "float" || config.valType == "int")
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

				if (config.valType == "float")
				{
					config.val = hexStr(reinterpret_cast<BYTE*>(&f), sizeof(float));
				}
				else if (config.valType == "int")
				{
					int i = static_cast<int>(f);
					config.val = hexStr(reinterpret_cast<BYTE*>(&i), sizeof(int));
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
			if (BytesFromString(config.val).size() == 0)
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
		std::string moduleName;
		if (config.module == "auto")
		{
			moduleName = szFileName;
		}
		else
		{
			moduleName = config.module;
		}

		mInfo = GetModuleInfo(const_cast<char*>(moduleName.c_str()));

		if (mInfo.EntryPoint == nullptr)
		{
			DBOUT("Could not find module " << moduleName << ", skipping patch...");
			continue;
		}

		auto pattern = hook::pattern((uintptr_t)mInfo.lpBaseOfDll, (uintptr_t)mInfo.lpBaseOfDll + mInfo.SizeOfImage, config.pattern);
		DBOUT("Found " << pattern.size() << " matches");

		if (pattern.size() == 0)
		{
			continue;
		}

		auto value = BytesFromString(config.val);
		if (config.matches == "all")
		{
			for (int i = 0; i < pattern.size(); i++)
			{
				DBOUT("Writing " << hexStr(value.data(), value.size()) << "with length " << value.size() << " to result " << i + 1);
				Memory::Write(pattern.get(i).get<BYTE>(config.offset), value.data(), value.size());
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

			DBOUT("Writing " << hexStr(value.data(), value.size()) << "with length " << value.size() << " to match " << match);
			Memory::Write(pattern.get(match - 1).get<BYTE>(config.offset), value.data(), value.size());
		}
	}
}