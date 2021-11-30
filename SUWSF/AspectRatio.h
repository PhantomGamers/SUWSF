#pragma once
#include <string>

class AspectRatio
{
public:
	static void Init();
private:
	struct Config
	{
		std::string bytes;
		int desiredWidth, desiredHeight;
		bool enabled = false;
	};
	static Config GetConfig();
	static void Patch(Config config);
};
