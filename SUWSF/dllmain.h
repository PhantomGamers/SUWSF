#pragma once
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string/case_conv.hpp>

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

// from https://stackoverflow.com/a/3221193
std::vector<unsigned char> BytesFromString(const std::string& s)
{
	std::istringstream hex_chars_stream(s);
	std::vector<unsigned char> bytes;

	unsigned int c;
	while (hex_chars_stream >> std::hex >> c)
	{
		bytes.push_back(c);
	}
	return bytes;
}

// from https://stackoverflow.com/a/14051107
std::string hexStr(BYTE* data, int len)
{
	std::stringstream ss;
	ss << std::hex;

	for (int i(0); i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i] << ' ';

	return boost::to_upper_copy(ss.str());
}