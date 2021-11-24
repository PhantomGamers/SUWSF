#pragma once
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

const std::string APPNAME("SUWSF");
const std::string VERSION("1.2.0");

// TODO: Separate this stuff out

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = {nullptr};
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == nullptr)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

// modified, original from https://stackoverflow.com/a/3221193
std::string HexFromString(const std::string& s)
{
	std::istringstream hex_chars_stream(s);
	std::string bytes;

	unsigned int c;
	while (hex_chars_stream >> std::hex >> c)
	{
		bytes += c;
	}
	return bytes;
}
