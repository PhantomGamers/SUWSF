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
		bool enabled = true;
	};
	static Config config;
private:
	static void SetConfig();
};
