#pragma once
#include <string>

class UserSettings
{
public:
	static void Init();
	struct Config
	{
		int width, height;
		double aspectratio;
		bool enabled = true;
		bool createThread = true;
		int delayInSeconds = 0;
	};
	static Config config;
private:
	static void SetConfig();
};
