#pragma once
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <sstream>
#include <vector>
#include <string>

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << '[' << APPNAME << "] " << s << '\n';                   \
   OutputDebugString( os_.str().c_str() );  \
}

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

// from https://stackoverflow.com/a/3221193
std::vector<char> StringToHex(const std::string& s)
{
	std::istringstream hex_chars_stream(s);
	std::vector<char> bytes;

	unsigned int c;
	while (hex_chars_stream >> std::hex >> c)
	{
		bytes.push_back(c);
	}
	return bytes;
}