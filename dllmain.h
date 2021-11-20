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

// TODO: Separate this stuff out

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

// from https://stackoverflow.com/a/27511119
std::vector<std::string> split(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(std::move(item));
	}
	return elems;
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

