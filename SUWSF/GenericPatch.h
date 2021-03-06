#pragma once
#include <string>
#include<vector>

class GenericPatch
{
public:
	static void Init();
private:
	struct Config
	{
		std::string pattern = "";
		int offset = 0;
		std::string val = "";
		std::string valType = "float";
		std::string matches = "all";
		std::string module = "auto";
	};
	static std::vector<Config> GetConfigs();
	static void PatchAll(std::vector<Config> configs);
};
