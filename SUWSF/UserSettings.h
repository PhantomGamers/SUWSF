#pragma once
#include <string>

class UserSettings
{
public:
	static void Init();
	struct Config
	{
		int width, height;
		float aspectratio;
		bool isSet = false;
	};
	static Config config;
private:
	static void SetConfig();
};
