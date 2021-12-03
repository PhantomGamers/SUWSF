#pragma once
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

#define STRINGIZER(arg)     #arg
#define STR_VALUE(arg)      STRINGIZER(arg)
#define BUILD_VERSION_STRING STR_VALUE(BUILD_VERSION)

const std::string APPNAME("SUWSF");
const std::string ININAME(APPNAME + ".ini");

const std::string VERSION = BUILD_VERSION_STRING;

TCHAR szFileName[MAX_PATH];
MODULEINFO mInfo;

// TODO: Separate this stuff out

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { nullptr };
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

// from https://codereview.stackexchange.com/a/78539
constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
						   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hexStr(unsigned char* data, int len)
{
	std::string s(len * 2, ' ');
	for (int i = 0; i < len; ++i) {
		s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return s;
}